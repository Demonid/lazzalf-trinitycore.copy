/* Copyright (C) 2010 EmeraldEmu <wow.4vendeta.com>
 * Emerald Emu is based on Trinity Core and MaNGOS. 
 * EEMU is personaly developed for Vendeta High Rate.
 */
 
#include "ScriptedPch.h"
#include "def.h"
 
enum
{
   SAY_AGGRO = -1900554,
   SAY_DEATH = -1900564,
   SAY_M	= -1900555,
   SAY_A = -1900556,
   SAY_S = -1900556,
   SAY_D = -1900560,
   
   SPELL_BERSERK = 26662,
   SPELL_PENETRATING_COLD = 67700,
   SPELL_PENETRATING_COLD_H = 68509,
   SPELL_PURSUIT = 65922,
   SPELL_LEECHING_SWARM = 68646,
   SPELL_LEECHING_SWARM_H = 68647,
   SPELL_SUBMERGE = 53421,
   SPELL_FREEZE_SLASH = 66012,
   SPELL_SHADOW_STRIKE = 66134
};

 
#define SPAWNPOINT_Z 143
 
struct  boss_anub_arak_crusaderAI : public ScriptedAI
{
    boss_anub_arak_crusaderAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        HeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }
 
    ScriptedInstance* pInstance;
 
    bool HeroicMode;
    bool Submerged;
    bool Burrower;
 
    uint32 AlternateUnderGround_Timer;
    uint32 AlternateOnGround_Timer;
    uint32 Freeze_Slash_Timer;
    uint32 Penetrating_Cold_Timer;
    uint32 Pursuit_Timer;
    uint32 Leeching_Swarm_Timer;
    uint8 Phase;
 
    void Reset()
    {
    AlternateUnderGround_Timer = 22500;
    AlternateOnGround_Timer = 45000;
    Freeze_Slash_Timer = 15000;
    Penetrating_Cold_Timer = 30000;
    Pursuit_Timer = 5000;
    Leeching_Swarm_Timer = 25000;
    Submerged = false;
    Burrower = false;
    Phase = 0;
    me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
    me->SetReactState(REACT_AGGRESSIVE);
    me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
    }
 
    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO,me);
        if(pInstance)
            pInstance->SetData(DATA_BOSS_ANUBARAK, IN_PROGRESS);
    }
 
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, me);
        if (pInstance)
            pInstance->SetData(DATA_BOSS_ANUBARAK, DONE);
    }
 
    void JustSummoned(Creature* mob)
    {
	mob->AddThreat(me->getVictim(), 0);
    }
 
    void UpdateAI(const uint32 diff)
    {
	if (!UpdateVictim())
		return;
 
    if ((me->GetHealth()*100 / me->GetMaxHealth()) > 90)
    {
	Phase = 1;
	}
    else
	{
	   if (((me->GetHealth()*100 / me->GetMaxHealth()) < 90) &&
	   ((me->GetHealth()*100 / me->GetMaxHealth()) > 30))
	{
	Phase = 2;
	}
    else Phase = 3;
    }
 
    if (Phase == 1 && !Submerged)
    {
	  if (Freeze_Slash_Timer < diff)
	  {
	  DoCast(me->getVictim(), SPELL_FREEZE_SLASH);
	  Freeze_Slash_Timer = 12000;
	  } else Freeze_Slash_Timer -= diff;
 
	if (Penetrating_Cold_Timer < diff)
	{
	  DoCast(me, HeroicMode ? SPELL_PENETRATING_COLD_H : SPELL_PENETRATING_COLD);
	  Penetrating_Cold_Timer = 30000;
	} else Penetrating_Cold_Timer -= diff;
    }
 
    if (Phase == 2)
    {
	if ((AlternateUnderGround_Timer < diff) && (!Submerged))
	{
	  DoScriptText(SAY_S,me);
	  me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	  me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
	  DoCast(me, SPELL_SUBMERGE, false);
	  AlternateUnderGround_Timer = 45000;
	  Submerged = true;
	  Burrower = true;
	} else AlternateUnderGround_Timer -= diff;
 
	if ((AlternateOnGround_Timer < diff) && (Submerged))
	{
	  me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	  me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
	  me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
	  AlternateOnGround_Timer = 45000;
	  Submerged = false;
	  Burrower = false;
	} else AlternateOnGround_Timer -= diff;
 
	if (Submerged && Burrower)
	{	
	  for(uint8 i = 0; i<1; i++)
	  {
	    if (Creature *pBoss=me->SummonCreature(NPC_NERUBIAN_BURROWER,756.5, 69.7033,SPAWNPOINT_Z,0,TEMPSUMMON_CORPSE_DESPAWN,120000))
	      {
	       if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
	           pBoss->AI()->AttackStart(pTarget);
	       }
	    
	    if (Creature *pTemp=me->SummonCreature(NPC_NERUBIAN_BURROWER,727.7, 69.5019,SPAWNPOINT_Z,0,TEMPSUMMON_CORPSE_DESPAWN,120000))
	      {
		if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
		    pTemp->AI()->AttackStart(pTarget);
	      }

	    break;
	  }
	  Burrower = false;
	}
 
	if (Pursuit_Timer < diff && Submerged)
	{
	  DoCast(me, SPELL_PURSUIT);
	  Pursuit_Timer = 9500;
	}else Pursuit_Timer -= diff;
    }
 
    if (Phase == 3 && !Submerged)
    {
	if (Freeze_Slash_Timer < diff)
	{
	  DoCast(me->getVictim(), SPELL_FREEZE_SLASH);
	  Freeze_Slash_Timer = 12000;
	} else Freeze_Slash_Timer -= diff;
 
	if (Penetrating_Cold_Timer < diff)
	{
	  DoCast(me, HeroicMode ? SPELL_PENETRATING_COLD_H : SPELL_PENETRATING_COLD);
	  Penetrating_Cold_Timer = 40000;
	} else Penetrating_Cold_Timer -= diff;
 
	if (Leeching_Swarm_Timer < diff)
	{
	  DoCast(me, HeroicMode ? SPELL_LEECHING_SWARM_H : SPELL_LEECHING_SWARM);
	  Leeching_Swarm_Timer = 10000;
	} else Leeching_Swarm_Timer -= diff;
    }
    if (!Submerged)
    DoMeleeAttackIfReady();
    }
};
 
struct  npc_borrowerAI : public ScriptedAI
{
    npc_borrowerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        HeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }
 
	ScriptedInstance* pInstance;
 
    bool mobSubmerged;
    bool HeroicMode;
 
    uint32 ShadowStrike_Timer;
    uint32 Submerge_Timer;
    uint32 Emerge_Timer;
 
    void Reset()
    {
    ShadowStrike_Timer = 5500;
    Submerge_Timer = 35000;
    Emerge_Timer = 10000;
    mobSubmerged = false;
    me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
    }
 
    void UpdateAI(const uint32 diff)
    {
	if (!UpdateVictim())
		return;
 
    Unit* pTarget = me->getVictim();
 
    if (ShadowStrike_Timer < diff && !mobSubmerged)
	{
	  DoCast(pTarget, SPELL_SHADOW_STRIKE);
	  ShadowStrike_Timer = 5500;
	} 
	else 
	ShadowStrike_Timer -= diff;
 
    if (Submerge_Timer < diff && !mobSubmerged)
	{
	me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
	DoCast(me, SPELL_SUBMERGE, false);
	Submerge_Timer = 35000;
	mobSubmerged = true;
	} 
	else 
	Submerge_Timer -= diff;
 
    if (Emerge_Timer < diff && mobSubmerged)
	{
	me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
	me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
	Emerge_Timer = 10000;
	mobSubmerged = false;
	} 
	else 
	Emerge_Timer -= diff;
 
    if (!mobSubmerged)
	DoMeleeAttackIfReady();
    }
};
 
CreatureAI* GetAI_boss_anub_arak_crusader(Creature* pCreature)
{
    return new boss_anub_arak_crusaderAI(pCreature);
}
 
CreatureAI* GetAI_npc_borrower(Creature* pCreature)
{
    return new npc_borrowerAI(pCreature);
}
 
void AddSC_boss_anub_arak_crusader()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_anub_arak_crusader";
    newscript->GetAI = &GetAI_boss_anub_arak_crusader;
    newscript->RegisterSelf();
 
newscript = new Script;
    newscript->Name = "npc_borrower";
    newscript->GetAI = &GetAI_npc_borrower;
    newscript->RegisterSelf();
}