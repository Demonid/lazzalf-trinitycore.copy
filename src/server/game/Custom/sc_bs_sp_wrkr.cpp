/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "ScriptPCH.h"
#include "sc_bs_sp_wrkr.h"
#include "ace/Process_Mutex.h"

#ifdef DEF_BOSS_SPELL_WORKER_H

void BossSpellWorker::resetTimer(uint32 SpellID)
{
    return _resetTimer(FindSpellIDX(SpellID));
};

void BossSpellWorker::resetTimers()
{
    for (uint8 i = 0; i < _bossSpellCount; ++i)
    _resetTimer(i);
};

bool BossSpellWorker::timedQuery(uint32 SpellID, uint32 diff)
{
    return _QuerySpellPeriod(FindSpellIDX(SpellID), diff);
};

CanCastResult BossSpellWorker::timedCast(uint32 SpellID, uint32 diff, Unit* pTarget)
{
    uint8 m_uiSpellIdx = FindSpellIDX(SpellID);
    if (!_QuerySpellPeriod(FindSpellIDX(SpellID), diff)) 
        return CAST_FAIL_STATE;
    else 
        return _BSWSpellSelector(m_uiSpellIdx, pTarget);
};

CanCastResult BossSpellWorker::doCast(uint32 SpellID, Unit* pTarget)
{
    uint8 m_uiSpellIdx = FindSpellIDX(SpellID);
    if ( m_uiSpellIdx != SPELL_INDEX_ERROR) 
        return _BSWSpellSelector(m_uiSpellIdx, pTarget);
    else 
        return CAST_FAIL_OTHER;
};

CanCastResult BossSpellWorker::doCast(Unit* pTarget, uint32 SpellID)
{
    if (!pTarget) 
        return CAST_FAIL_OTHER;
    uint8 m_uiSpellIdx = FindSpellIDX(SpellID);
    if ( m_uiSpellIdx != SPELL_INDEX_ERROR) 
        return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
    else 
        return CAST_FAIL_OTHER;
};

bool BossSpellWorker::doRemove(uint32 SpellID, Unit* pTarget, SpellEffectIndex index)
{
    return _doRemove(FindSpellIDX(SpellID),pTarget, index);
};

bool BossSpellWorker::hasAura(uint32 SpellID, Unit* pTarget)
{
    if (!pTarget) 
        pTarget = boss;
    return _hasAura(FindSpellIDX(SpellID),pTarget);
};

Unit* BossSpellWorker::doSummon(uint32 SpellID, TempSummonType type, uint32 delay)
{
    return _doSummon(FindSpellIDX(SpellID), type, delay);
};

Unit* BossSpellWorker::SelectRandomPlayer(uint32 SpellID, bool spellsearchtype, float range)
{
    return _doSelect(SpellID, spellsearchtype, range);
};

Unit* BossSpellWorker::SelectRandomPlayerAtRange(float range)
{
    return _doSelect(0, false, range);
};

Unit* BossSpellWorker::doSummon(uint32 SpellID, float fPosX, float fPosY, float fPosZ, TempSummonType type, uint32 delay)
{
    return _doSummonAtPosition(FindSpellIDX(SpellID), type, delay, fPosX, fPosY, fPosZ);
};

CanCastResult BossSpellWorker::BSWSpellSelector(uint32 SpellID, Unit* pTarget)
{
    return _BSWSpellSelector(FindSpellIDX(SpellID), pTarget);
};

CanCastResult BossSpellWorker::BSWDoCast(uint32 SpellID, Unit* pTarget)
{
    return _BSWDoCast(FindSpellIDX(SpellID), pTarget);
};

Unit*  BossSpellWorker::SelectUnit(SelectAggroTarget target, uint32 uiPosition)
{
    return  _SelectUnit(target, uiPosition);
};

uint8 BossSpellWorker::bossSpellCount()
{
    return _bossSpellCount;
};

BossSpellWorker::BossSpellWorker(ScriptedAI* bossAI)
{
    boss = bossAI->me;
    bossID = boss->GetEntry();
    _bossSpellCount = 0;
    currentTarget = NULL;
    memset(&m_uiSpell_Timer, 0, sizeof(m_uiSpell_Timer));
    memset(&m_BossSpell,0,sizeof(m_BossSpell));
    if (pMap = boss->GetMap())
    currentDifficulty = ((InstanceMap*)pMap)->GetDifficulty();
    else 
         currentDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;
    sLog->outDebug("BSW: Initializing BossSpellWorker object for boss %u difficulty %u",bossID,currentDifficulty);
    LoadSpellTable();
    Reset((uint8)currentDifficulty);
};

BossSpellWorker::~BossSpellWorker()
{
     sLog->outDebug("BSW: Removing BossSpellWorker object for boss %u",bossID);
};

void BossSpellWorker::Reset(uint8 _Difficulty)
{
    currentDifficulty = setDifficulty(_Difficulty);
    resetTimers();
};

void BossSpellWorker::_resetTimer(uint8 m_uiSpellIdx)
{
    if (m_uiSpellIdx > _bossSpellCount) 
        return;

    if (m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMin[currentDifficulty] != m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMax[currentDifficulty])
        m_uiSpell_Timer[m_uiSpellIdx] = urand(0,m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMax[currentDifficulty]);
    else 
        m_uiSpell_Timer[m_uiSpellIdx] = m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMin[currentDifficulty];

    if (m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMin[currentDifficulty] == 0 
        && m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMax[currentDifficulty] >= HOUR*IN_MILLISECONDS)
            m_uiSpell_Timer[m_uiSpellIdx] = 0;
};

void BossSpellWorker::LoadSpellTable()
{
    sLog->outDebug("BSW: Loading table of spells boss  %u difficulty %u", bossID , currentDifficulty);

      char query[MAX_QUERY_LEN];

      sprintf(query, "SELECT entry, spellID_N10, spellID_N25, spellID_H10, spellID_H25, timerMin_N10, timerMin_N25, timerMin_H10, timerMin_H25, timerMax_N10, timerMax_N25, timerMax_H10, timerMax_H25, data1, data2, data3, data4, locData_x, locData_y, locData_z, varData, StageMask_N, StageMask_H, CastType, isVisualEffect, isBugged, textEntry FROM `boss_spell_table` WHERE entry = %u;\r\n", bossID);

      QueryResult Result = WorldDatabase.PQuery(query);

    if (Result)
    {
        uint32 uiCount = 0;
        do
        {
            Field* pFields = Result->Fetch();

            m_BossSpell[uiCount].id  = uiCount;

            int32 bossEntry          = pFields[0].GetInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellEntry[j]  = pFields[1+j].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellTimerMin[j]  = pFields[1+DIFFICULTY_LEVELS+j].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellTimerMax[j]  = pFields[1+DIFFICULTY_LEVELS*2+j].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellData[j]  = pFields[1+DIFFICULTY_LEVELS*3+j].GetUInt32();

            m_BossSpell[uiCount].LocData.x  = pFields[1+DIFFICULTY_LEVELS*4].GetFloat();
            m_BossSpell[uiCount].LocData.y  = pFields[2+DIFFICULTY_LEVELS*4].GetFloat();
            m_BossSpell[uiCount].LocData.z  = pFields[3+DIFFICULTY_LEVELS*4].GetFloat();

            m_BossSpell[uiCount].varData    = pFields[4+DIFFICULTY_LEVELS*4].GetInt32();

            m_BossSpell[uiCount].StageMaskN = pFields[5+DIFFICULTY_LEVELS*4].GetUInt32();
            m_BossSpell[uiCount].StageMaskH = pFields[6+DIFFICULTY_LEVELS*4].GetUInt32();

            m_BossSpell[uiCount].m_CastTarget = getBSWCastType(pFields[7+DIFFICULTY_LEVELS*4].GetUInt8());

            m_BossSpell[uiCount].m_IsVisualEffect = (pFields[8+DIFFICULTY_LEVELS*4].GetUInt8() == 0) ? false : true ;

            m_BossSpell[uiCount].m_IsBugged = (pFields[9+DIFFICULTY_LEVELS*4].GetUInt8() == 0) ? false : true ;

            m_BossSpell[uiCount].textEntry = pFields[10+DIFFICULTY_LEVELS*4].GetInt32();

            if (bossEntry != bossID) sLog->outErrorDb("BSW: Unknown error while load boss_spell_table");

            ++uiCount;

        } while (Result->NextRow());

        _bossSpellCount = uiCount;

        _fillEmptyDataField();

        sLog->outDebug("BSW: Loaded %u boss spell data records for boss %u", uiCount, bossID);
    }
    else
    {
        sLog->outErrorDb("BSW: Boss spell table for boss %u is empty.", bossID);
        _bossSpellCount = 0;
    };
}

bool BossSpellWorker::_QuerySpellPeriod(uint8 m_uiSpellIdx, uint32 diff)
{
    if (_bossSpellCount == 0) 
        return false;

    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (m_uiSpell_Timer[m_uiSpellIdx] < diff) 
    {
        if (pSpell->m_uiSpellTimerMax[currentDifficulty] >= HOUR*IN_MILLISECONDS) 
            m_uiSpell_Timer[m_uiSpellIdx] = HOUR*IN_MILLISECONDS;
        else 
            m_uiSpell_Timer[m_uiSpellIdx] = urand(pSpell->m_uiSpellTimerMin[currentDifficulty],pSpell->m_uiSpellTimerMax[currentDifficulty]);
        return true;
    }
    else 
    {
        m_uiSpell_Timer[m_uiSpellIdx] -= diff;
        return false;
    }
};

CanCastResult BossSpellWorker::_BSWSpellSelector(uint8 m_uiSpellIdx, Unit* pTarget)
{
    if (_bossSpellCount == 0) return CAST_FAIL_OTHER;
    SpellEntry const *spell;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
    Unit* pSummon = NULL;

        sLog->outDebug("BSW: Casting spell number %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

        switch (pSpell->m_CastTarget) {

            case DO_NOTHING: 
                   return CAST_OK;

            case CAST_ON_SELF:
                   if (!pSpell->m_IsBugged) return _DoCastSpellIfCan(boss, pSpell->m_uiSpellEntry[currentDifficulty]);
                   else return _BSWDoCast(m_uiSpellIdx, boss);
                   break;

            case CAST_ON_SUMMONS:
                   if (!pTarget) return CAST_FAIL_OTHER;
                   else return _DoCastSpellIfCan(pTarget, pSpell->m_uiSpellEntry[currentDifficulty]);
                   break;

            case CAST_ON_VICTIM:
                   pTarget = boss->getVictim();
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_RANDOM:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM);
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_BOTTOMAGGRO:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_TARGET:
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case APPLY_AURA_SELF:
                   if (spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[currentDifficulty]))
                          if(boss->AddAura(pSpell->m_uiSpellEntry[currentDifficulty], boss))
                              return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case APPLY_AURA_TARGET:
                   if (!pTarget) return CAST_FAIL_OTHER;
                   if (spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[currentDifficulty]))
                          if (pTarget->AddAura(pSpell->m_uiSpellEntry[currentDifficulty], pTarget))
                              return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case SUMMON_NORMAL:
                   pSummon = _doSummon(m_uiSpellIdx, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                   if(pSummon) return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case SUMMON_TEMP:
                   pSummon = _doSummon(m_uiSpellIdx, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,
                                        urand(pSpell->m_uiSpellTimerMin[currentDifficulty],pSpell->m_uiSpellTimerMax[currentDifficulty]));
                   if(pSummon) return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case SUMMON_INSTANT:
                   pSummon = _doSummon(m_uiSpellIdx, TEMPSUMMON_MANUAL_DESPAWN,0);
                   if(pSummon) return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case CAST_ON_ALLPLAYERS:
            {
                    CanCastResult res1 = CAST_FAIL_OTHER;
                    Map::PlayerList const& pPlayers = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        pTarget = itr->getSource();
                        if (pTarget && pTarget->isAlive() && pTarget->IsWithinDistInMap(boss, pSpell->LocData.x))
                                    if (!pSpell->m_IsBugged) {
                                         res1 = _DoCastSpellIfCan(pTarget, pSpell->m_uiSpellEntry[currentDifficulty]);
                                         }
                                         else {
                                          _BSWDoCast(m_uiSpellIdx, pTarget);
                                          res1 = CAST_OK;
                                          };
                            return res1;
                     }
                   break;
            }

            case CAST_ON_FRENDLY:
                   pTarget = SelectLowHPFriendly();
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_FRENDLY_LOWHP:
                   pTarget = SelectLowHPFriendly();
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            default:
                   return CAST_FAIL_OTHER;
                   break;
            };
    return CAST_FAIL_OTHER;
};

CanCastResult BossSpellWorker::_BSWCastOnTarget(Unit* pTarget, uint8 m_uiSpellIdx)
{
    if (_bossSpellCount == 0) return CAST_FAIL_OTHER;
    if (!pTarget)            return CAST_FAIL_OTHER;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    sLog->outDebug("BSW: Casting (on target) spell number %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

       if (pTarget && !pSpell->m_IsBugged) return _DoCastSpellIfCan(pTarget, pSpell->m_uiSpellEntry[currentDifficulty]);
       if (pTarget && pSpell->m_IsBugged) return _BSWDoCast(m_uiSpellIdx, pTarget);

    return CAST_FAIL_OTHER;
};



bool BossSpellWorker::isSummon(uint8 m_uiSpellIdx)
{
    if (_bossSpellCount == 0) return false;

    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
    switch (pSpell->m_CastTarget) {
        case SUMMON_NORMAL:
        case SUMMON_TEMP:
        case SUMMON_INSTANT:
                             return true;
        default: return false;
        };
};

bool BossSpellWorker::_hasAura(uint8 m_uiSpellIdx, Unit* pTarget)
{
    if (!pTarget) return false;

    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    return (pTarget->HasAura(pSpell->m_uiSpellEntry[currentDifficulty]));

};

uint8 BossSpellWorker::FindSpellIDX(uint32 SpellID)
{
    if (_bossSpellCount != 0)
      for(uint8 i = 0; i < _bossSpellCount; ++i)
        if (m_BossSpell[i].m_uiSpellEntry[RAID_DIFFICULTY_10MAN_NORMAL] == SpellID) return i;

    sLog->outDebug("BSW: spell %u not found  in boss %u spelltable. Memory or database error?", SpellID, bossID);
    return SPELL_INDEX_ERROR;
}

Difficulty BossSpellWorker::setDifficulty(uint8 _Difficulty)
{
switch (_Difficulty) {
        case RAID_DIFFICULTY_10MAN_NORMAL:
                return RAID_DIFFICULTY_10MAN_NORMAL;
        case RAID_DIFFICULTY_25MAN_NORMAL:
                return RAID_DIFFICULTY_25MAN_NORMAL;
        case RAID_DIFFICULTY_10MAN_HEROIC:
                return RAID_DIFFICULTY_10MAN_HEROIC;
        case RAID_DIFFICULTY_25MAN_HEROIC:
                return RAID_DIFFICULTY_25MAN_HEROIC;
        default:
                return RAID_DIFFICULTY_10MAN_NORMAL;
        };
}

BossSpellTableParameters BossSpellWorker::getBSWCastType(uint32 pTemp)
{
    switch (pTemp) {
                case 0:  return DO_NOTHING;
                case 1:  return CAST_ON_SELF;
                case 2:  return CAST_ON_SUMMONS;
                case 3:  return CAST_ON_VICTIM;
                case 4:  return CAST_ON_RANDOM;
                case 5:  return CAST_ON_BOTTOMAGGRO;
                case 6:  return CAST_ON_TARGET;
                case 7:  return APPLY_AURA_SELF;
                case 8:  return APPLY_AURA_TARGET;
                case 9:  return SUMMON_NORMAL;
                case 10: return SUMMON_INSTANT;
                case 11: return SUMMON_TEMP;
                case 12: return CAST_ON_ALLPLAYERS;
                case 13: return CAST_ON_FRENDLY;
                case 14: return CAST_ON_FRENDLY_LOWHP;
                case 15: return SPELLTABLEPARM_NUMBER;
     default: return DO_NOTHING;
     };
};

CanCastResult BossSpellWorker::_BSWDoCast(uint8 m_uiSpellIdx, Unit* pTarget)
{
    if (!pTarget) return CAST_FAIL_OTHER;
    if (!pTarget->isAlive()) return CAST_FAIL_OTHER;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
    sLog->outDebug("BSW: Casting bugged spell number %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);
    pTarget->InterruptNonMeleeSpells(false);
    pTarget->CastSpell(pTarget, pSpell->m_uiSpellEntry[currentDifficulty], false);
         return CAST_OK;
};

void BossSpellWorker::_fillEmptyDataField()
{
    for (uint8 i = 0; i < _bossSpellCount; ++i)
        for (uint8 j = 1; j < DIFFICULTY_LEVELS; ++j)
        {
            if (m_BossSpell[i].m_uiSpellEntry[j] == 0)
                   m_BossSpell[i].m_uiSpellEntry[j] = m_BossSpell[i].m_uiSpellEntry[j-1];

            if (m_BossSpell[i].m_uiSpellTimerMin[j] == 0)
                   m_BossSpell[i].m_uiSpellTimerMin[j] = m_BossSpell[i].m_uiSpellTimerMin[j-1];

            if (m_BossSpell[i].m_uiSpellTimerMax[j] == 0)
                   m_BossSpell[i].m_uiSpellTimerMax[j] = m_BossSpell[i].m_uiSpellTimerMax[j-1];

            if (m_BossSpell[i].m_uiSpellData[j] == 0)
                   m_BossSpell[i].m_uiSpellData[j] = m_BossSpell[i].m_uiSpellData[j-1];
        };
};

Unit* BossSpellWorker::_doSummon(uint8 m_uiSpellIdx, TempSummonType summontype, uint32 delay)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    sLog->outDebug("BSW: Summoning creature number %u type %u despawn delay %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget, delay);

    if (pSpell->LocData.z <= 1.0f) {
    const Position pos = {boss->GetPositionX(), boss->GetPositionY(), boss->GetPositionZ()};
    Position dst;
    boss->GetRandomPoint(pos, float(urand((uint32)pSpell->LocData.x, (uint32)pSpell->LocData.y)), dst);
    return boss->SummonCreature(pSpell->m_uiSpellEntry[currentDifficulty], dst, summontype, delay);
    } else return boss->SummonCreature(pSpell->m_uiSpellEntry[currentDifficulty], pSpell->LocData.x, pSpell->LocData.y, pSpell->LocData.z, 0, summontype, delay);
};

Unit* BossSpellWorker::_doSummonAtPosition(uint8 m_uiSpellIdx, TempSummonType summontype, uint32 delay, float fPosX, float fPosY, float fPosZ)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    sLog->outDebug("BSW: Summoning creature number %u type %u despawn delay %u at position %f %f %f",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget, delay, fPosX, fPosY, fPosZ);
    return boss->SummonCreature(pSpell->m_uiSpellEntry[currentDifficulty], fPosX, fPosY, fPosZ, 0, summontype, delay);
};

bool BossSpellWorker::_doRemove(uint8 m_uiSpellIdx, Unit* pTarget, SpellEffectIndex index)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

        sLog->outDebug("BSW: Removing effect of spell %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

        switch (pSpell->m_CastTarget) {
                case DO_NOTHING: 
                                 return true;
                case SUMMON_NORMAL:
                case SUMMON_TEMP:
                case SUMMON_INSTANT:
                                 return false;

                case CAST_ON_SELF:
                case APPLY_AURA_SELF:
                     if (!pTarget) pTarget = boss;
                     break;

                case CAST_ON_SUMMONS:
                case CAST_ON_VICTIM:
                case CAST_ON_BOTTOMAGGRO:
                case CAST_ON_TARGET:
                case APPLY_AURA_TARGET:
                     if (!pTarget) return false;
                     break;

                case CAST_ON_RANDOM:
                case CAST_ON_ALLPLAYERS:
                  {
                    Map::PlayerList const& pPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                      {
                        pTarget = itr->getSource();
                        if (pTarget && pTarget->isAlive() && pTarget->IsWithinDistInMap(boss, pSpell->LocData.x))
                            pTarget->RemoveAurasDueToSpell(pSpell->m_uiSpellEntry[currentDifficulty]);
                       }
                        return true;
                   break;
                   }

                  default: return false;
          }
          if (pTarget) {
              if (pTarget->isAlive()) {
                  if ( pTarget->HasAura(pSpell->m_uiSpellEntry[currentDifficulty]) &&
                       pTarget->GetAura(pSpell->m_uiSpellEntry[currentDifficulty], index)->GetStackAmount() > 1) {
                           if (pTarget->GetAura(pSpell->m_uiSpellEntry[currentDifficulty], index)->ModStackAmount(-1)) 
                                         return true;
                               else return false;
                               }
                           else pTarget->RemoveAurasDueToSpell(pSpell->m_uiSpellEntry[currentDifficulty]);
                           }
                   else pTarget->RemoveAurasDueToSpell(pSpell->m_uiSpellEntry[currentDifficulty]);
                  }
     return true;
};

// Copypasting from CreatureAI.cpp. if this called from bossAI-> crashed :(

CanCastResult BossSpellWorker::_CanCastSpell(Unit* pTarget, const SpellEntry *pSpell, bool isTriggered)
{
    if (!pTarget) return CAST_FAIL_OTHER;
    // If not triggered, we check
    if (!isTriggered)
    {
        // State does not allow
        if (boss->HasUnitState(UNIT_STAT_CAN_NOT_REACT))
            return CAST_FAIL_STATE;

        if (pSpell->PreventionType == SPELL_PREVENTION_TYPE_SILENCE && boss->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
            return CAST_FAIL_STATE;

        if (pSpell->PreventionType == SPELL_PREVENTION_TYPE_PACIFY && boss->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED))
            return CAST_FAIL_STATE;

        // Check for power (also done by Spell::CheckCast())
        if (boss->GetPower((Powers)pSpell->powerType) < pSpell->manaCost)
            return CAST_FAIL_POWER;
    }

    if (const SpellRangeEntry *pSpellRange = GetSpellRangeStore()->LookupEntry(pSpell->rangeIndex))
    {
        if (pTarget != boss)
        {
            // pTarget is out of range of this spell (also done by Spell::CheckCast())
            float fDistance = boss->GetCombatDistance(pTarget);

            if (fDistance > (boss->IsHostileTo(pTarget) ? pSpellRange->maxRangeHostile : pSpellRange->maxRangeFriend))
                return CAST_FAIL_TOO_FAR;

            float fMinRange = boss->IsHostileTo(pTarget) ? pSpellRange->minRangeHostile : pSpellRange->minRangeFriend;

            if (fMinRange && fDistance < fMinRange)
                return CAST_FAIL_TOO_CLOSE;
        }

        return CAST_OK;
    }
    else
        return CAST_FAIL_OTHER;
}

CanCastResult BossSpellWorker::_DoCastSpellIfCan(Unit* pTarget, uint32 uiSpell, uint32 uiCastFlags, uint64 uiOriginalCasterGUID)
{
    Unit* pCaster = boss;
    if (!pTarget) return CAST_FAIL_OTHER;

    if (uiCastFlags & CAST_FORCE_TARGET_SELF)
        pCaster = pTarget;

    // Allowed to cast only if not casting (unless we interrupt ourself) or if spell is triggered
    if (!pCaster->IsNonMeleeSpellCasted(false) || (uiCastFlags & (CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS)))
    {
        if (const SpellEntry* pSpell = GetSpellStore()->LookupEntry(uiSpell))
        {
            // If cast flag CAST_AURA_NOT_PRESENT is active, check if target already has aura on them
            if (uiCastFlags & CAST_AURA_NOT_PRESENT)
            {
                if (pTarget->HasAura(uiSpell))
                    return CAST_FAIL_TARGET_AURA;
            }

            // Check if cannot cast spell
            if (!(uiCastFlags & (CAST_FORCE_TARGET_SELF | CAST_FORCE_CAST)))
            {
                CanCastResult castResult = _CanCastSpell(pTarget, pSpell, uiCastFlags & CAST_TRIGGERED);

                if (castResult != CAST_OK)
                    return castResult;
            }

            // Interrupt any previous spell
            if (uiCastFlags & CAST_INTERRUPT_PREVIOUS && pCaster->IsNonMeleeSpellCasted(false))
                pCaster->InterruptNonMeleeSpells(false);

            pCaster->CastSpell(pTarget, pSpell, uiCastFlags & CAST_TRIGGERED, NULL, NULL, uiOriginalCasterGUID);
            return CAST_OK;
        }
        else
        {
            sLog->outDebug("BSW: DoCastSpellIfCan by creature entry %u attempt to cast spell %u but spell does not exist.", boss->GetEntry(), uiSpell);
            return CAST_FAIL_OTHER;
        }
    }
    else
        return CAST_FAIL_IS_CASTING;
}

// Copypasting from sc_creature.cpp :( Hung if call from bossAI->

Unit*  BossSpellWorker::_SelectUnit(SelectAggroTarget target, uint32 uiPosition)
{
    //ThreatList m_threatlist;
    std::list<HostileReference*> const& threatlist = boss->getThreatManager().getThreatList();
    std::list<HostileReference*>::const_iterator itr = threatlist.begin();
    std::list<HostileReference*>::const_reverse_iterator ritr = threatlist.rbegin();

    if (uiPosition >= threatlist.size() || threatlist.empty())
        return NULL;

    switch (target)
    {
        case SELECT_TARGET_RANDOM:
            advance(itr, uiPosition +  (rand() % (threatlist.size() - uiPosition)));
            return Unit::GetUnit((*boss),(*itr)->getUnitGuid());
            break;

        case SELECT_TARGET_TOPAGGRO:
            advance(itr, uiPosition);
            return Unit::GetUnit((*boss),(*itr)->getUnitGuid());
            break;

        case SELECT_TARGET_BOTTOMAGGRO:
            advance(ritr, uiPosition);
            return Unit::GetUnit((*boss),(*ritr)->getUnitGuid());
            break;
    }

    sLog->outDebug("BSW: Cannot find target for spell :(");
    return NULL;
}

Unit* BossSpellWorker::SelectLowHPFriendly(float fRange, uint32 uiMinHPDiff)
{
    CellPair p(Trinity::ComputeCellPair(boss->GetPositionX(), boss->GetPositionY()));
    Cell cell(p);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    Unit* pUnit = NULL;

    Trinity::MostHPMissingInRange u_check(boss, fRange, uiMinHPDiff);
    Trinity::UnitLastSearcher<Trinity::MostHPMissingInRange> searcher(boss, pUnit, u_check);

    TypeContainerVisitor<Trinity::UnitLastSearcher<Trinity::MostHPMissingInRange>, GridTypeMapContainer >  grid_unit_searcher(searcher);

    cell.Visit(p, grid_unit_searcher, *(pMap), *boss, fRange);

    return pUnit;
}

// Not threat-based select random player function

Unit* BossSpellWorker::_doSelect(uint32 SpellID, bool spellsearchtype, float range)
{
    Map::PlayerList const &pList = pMap->GetPlayers();
          if (pList.isEmpty()) return NULL;

#if defined( __GNUC__ )
    Unit* _list[((InstanceMap*)pMap)->GetMaxPlayers()];
#else
    Unit* _list[INSTANCE_MAX_PLAYERS];
#endif 
    uint8 _count = 0;

    memset(&_list, 0, sizeof(_list));


          for(Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
          {
              if (Player* player = i->getSource())
                 {
                  if (player->isGameMaster()) continue;

                  if ( player->isAlive()
                       && player->IsWithinDistInMap(boss, range)
                       && (SpellID == 0 || (player->HasAura(SpellID) == spellsearchtype))
                     )
                     {
                     _list[_count] = (Unit*)player;
                     ++_count;
                     }
                 }
           }
    sLog->outDebug("BSW: search result for random player, count = %u ",_count);
    if (_count == 0) return NULL;
    else return _list[urand(0,_count)];
};

#endif
/* GUDE: ERRORS EXAMPLE!

#include "precompiled.h" to  #include "ScriptedPch.h"

pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0); -> pTarget = bsw->SelectUnit();
pTarget= me->SelectAttackingTarget(SELECT_TARGET_RANDOM, 0) -> Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0) )

m_creature->AddSplineFlag -> AddUnitMovementFlag (unit.cpp enum MovementFlags )

if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()) -> if (!UpdateVictim())

m_creature->SetSpeedRate -> me->SetSpeed()

***(unit.cpp enum MovementFlags ) ***
me->AddSplineFlag(SPLINEFLAG_WALKMODE); -> me->AddUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);

ThreatList -> std::list<HostileReference*>

ATTACKING_TARGET_RANDOM -> SELECT_TARGET_RANDOM

 me->AddThreat(pWho); -> me->AddThreat(pWho, 10.0f);

me->RemoveSpellsCausingAura to me->RemoveAurasByType


*/