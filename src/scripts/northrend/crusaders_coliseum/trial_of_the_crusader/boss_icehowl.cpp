/* Copyright (C) 2010 EmeraldEmu <wow.4vendeta.com>
 * Emerald Emu is based on Trinity Core and MaNGOS. 
 * EEMU is personaly developed for Vendeta High Rate.
 */

#include "ScriptedPch.h"
#include "def.h"

enum Spells
{
SPELL_FEROICIOUS_BUTT	= 66770, //бодание
SPELL_FEROICIOUS_BUTT_H = 67656,
SPELL_MASSIVE_CRASH	= 66683,
SPELL_MASSIVE_CRASH_H	= 67662, //сокрушение
SPELL_TRAMBLE		= 66734, //топот
SPELL_WHIRL		= 67345, //вращение
SPELL_WHIRL_H		= 67665,
SPELL_BERSERK		= 26662
};
#define SAY_AGGRO -1900506
#define SAY_DEATH -1900507
#define SAY_MASSIVE -1900508

struct  boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        HeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }
 
    ScriptedInstance* pInstance;
 
    bool HeroicMode;

   uint32 Feroicious_butt_Timer;
   uint32 Massive_crash_Timer;
   uint32 Tramble_Timer;
   uint32 Whirl_Timer;
   uint32 Berserk_Timer;
   
   void Reset()
    {
    Feroicious_butt_Timer = 40000;
    Massive_crash_Timer = 30000;
    Tramble_Timer	= 18000;
    Whirl_Timer		= 14000;
    Berserk_Timer	= 600000;
    }
    
    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO,m_creature);
        if(pInstance)
            pInstance->SetData(DATA_BOSS_ICEHOWL, IN_PROGRESS);
    }
    
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
	if (pInstance)
            pInstance->SetData(DATA_BOSS_ICEHOWL, DONE);
    }
    
    void UpdateAI(const uint32 diff)
    {

	if (!UpdateVictim())
		return;

	if (Tramble_Timer < diff)
	{
	DoCast(m_creature, SPELL_TRAMBLE);
	Tramble_Timer = 12000;
	}
	else
	Tramble_Timer -=diff;
	
	if (Feroicious_butt_Timer < diff)
	{
	DoCast(m_creature, HeroicMode ? SPELL_FEROICIOUS_BUTT_H : SPELL_FEROICIOUS_BUTT);
	Feroicious_butt_Timer = 40000;
	}
	else
	Feroicious_butt_Timer -=diff;
	
	if (Massive_crash_Timer < diff)
	{
	DoScriptText(SAY_MASSIVE, m_creature);
	DoCast(m_creature, HeroicMode ? SPELL_MASSIVE_CRASH_H : SPELL_MASSIVE_CRASH);
	Massive_crash_Timer = 30000;
	}
	else
	Massive_crash_Timer -=diff;
	
	if (Whirl_Timer < diff)
	{
	DoCast(m_creature, HeroicMode ? SPELL_WHIRL_H : SPELL_WHIRL);
	Whirl_Timer = 30000;
	}
	else
	Whirl_Timer -=diff;
	
	if (Berserk_Timer < diff)
	{
	DoCast(m_creature, SPELL_BERSERK);
	Berserk_Timer = 300000;
	}
	else
	Berserk_Timer -=diff;
	if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 10)
	{
	m_creature->SummonCreature(0, 604.996, 150.775, 138.629, 3.36);
	}
	
    DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}
 
void AddSC_boss_icehowl()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_icehowl";
    newscript->GetAI = &GetAI_boss_icehowl;
    newscript->RegisterSelf();
}
// update `creature_template` set `ScriptName`='boss_icehowl' where `entry` = '34797';