/*
 * Copyright (C) 2013  BlizzLikeGroup
 * BlizzLikeCore integrates as part of this file: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: SimpleAI
Complete(%): 100
Comment: Base Class for SimpleAI creatures
Category: Creatures
EndScriptData */

#include "ScriptPCH.h"
#include "ScriptedSimpleAI.h"

SimpleAI::SimpleAI(Creature* c) : ScriptedAI(c)
{
    //Clear all data
    Aggro_TextId[0] = 0;
    Aggro_TextId[1] = 0;
    Aggro_TextId[2] = 0;
    Aggro_Sound[0] = 0;
    Aggro_Sound[1] = 0;
    Aggro_Sound[2] = 0;

    Death_TextId[0] = 0;
    Death_TextId[1] = 0;
    Death_TextId[2] = 0;
    Death_Sound[0] = 0;
    Death_Sound[1] = 0;
    Death_Sound[2] = 0;
    Death_Spell = 0;
    Death_Target_Type = 0;

    Kill_TextId[0] = 0;
    Kill_TextId[1] = 0;
    Kill_TextId[2] = 0;
    Kill_Sound[0] = 0;
    Kill_Sound[1] = 0;
    Kill_Sound[2] = 0;
    Kill_Spell = 0;
    Kill_Target_Type = 0;

    memset(Spell,0,sizeof(Spell));

    EnterEvadeMode();
}

void SimpleAI::Reset()
{
}

void SimpleAI::EnterCombat(Unit* who)
{
    //Reset cast timers
    if (Spell[0].First_Cast >= 0)
        Spell_Timer[0] = Spell[0].First_Cast;
    else Spell_Timer[0] = 1000;
    if (Spell[1].First_Cast >= 0)
        Spell_Timer[1] = Spell[1].First_Cast;
    else Spell_Timer[1] = 1000;
    if (Spell[2].First_Cast >= 0)
        Spell_Timer[2] = Spell[2].First_Cast;
    else Spell_Timer[2] = 1000;
    if (Spell[3].First_Cast >= 0)
        Spell_Timer[3] = Spell[3].First_Cast;
    else Spell_Timer[3] = 1000;
    if (Spell[4].First_Cast >= 0)
        Spell_Timer[4] = Spell[4].First_Cast;
    else Spell_Timer[4] = 1000;
    if (Spell[5].First_Cast >= 0)
        Spell_Timer[5] = Spell[5].First_Cast;
    else Spell_Timer[5] = 1000;
    if (Spell[6].First_Cast >= 0)
        Spell_Timer[6] = Spell[6].First_Cast;
    else Spell_Timer[6] = 1000;
    if (Spell[7].First_Cast >= 0)
        Spell_Timer[7] = Spell[7].First_Cast;
    else Spell_Timer[7] = 1000;
    if (Spell[8].First_Cast >= 0)
        Spell_Timer[8] = Spell[8].First_Cast;
    else Spell_Timer[8] = 1000;
    if (Spell[9].First_Cast >= 0)
        Spell_Timer[9] = Spell[9].First_Cast;
    else Spell_Timer[9] = 1000;

    uint8 random_text = urand(0,2);

    //Random text
    if (Aggro_TextId[random_text])
        DoScriptText(Aggro_TextId[random_text], me, who);

    //Random sound
    if (Aggro_Sound[random_text])
        DoPlaySoundToSet(me, Aggro_Sound[random_text]);
}

void SimpleAI::KilledUnit(Unit* victim)
{
    uint8 random_text = urand(0,2);

    //Random yell
    if (Kill_TextId[random_text])
        DoScriptText(Kill_TextId[random_text], me, victim);

    //Random sound
    if (Kill_Sound[random_text])
        DoPlaySoundToSet(me, Kill_Sound[random_text]);

    if (!Kill_Spell)
        return;

    Unit* pTarget = NULL;

    switch (Kill_Target_Type)
    {
    case CAST_SELF:
        pTarget = me;
        break;
    case CAST_HOSTILE_TARGET:
        pTarget = me->getVictim();
        break;
    case CAST_HOSTILE_SECOND_AGGRO:
        pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
        break;
    case CAST_HOSTILE_LAST_AGGRO:
        pTarget = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
        break;
    case CAST_HOSTILE_RANDOM:
        pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
        break;
    case CAST_KILLEDUNIT_VICTIM:
        pTarget = victim;
        break;
    }

    //Target is ok, cast a spell on it
    if (pTarget)
        DoCast(pTarget, Kill_Spell);
}

void SimpleAI::DamageTaken(Unit* killer, uint32 &damage)
{
    //Return if damage taken won't kill us
    if (me->GetHealth() > damage)
        return;

    uint8 random_text = urand(0,2);

    //Random yell
    if (Death_TextId[random_text])
        DoScriptText(Death_TextId[random_text], me, killer);

    //Random sound
    if (Death_Sound[random_text])
        DoPlaySoundToSet(me, Death_Sound[random_text]);

    if (!Death_Spell)
        return;

    Unit* pTarget = NULL;

    switch (Death_Target_Type)
    {
    case CAST_SELF:
        pTarget = me;
        break;
    case CAST_HOSTILE_TARGET:
        pTarget = me->getVictim();
        break;
    case CAST_HOSTILE_SECOND_AGGRO:
        pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
        break;
    case CAST_HOSTILE_LAST_AGGRO:
        pTarget = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
        break;
    case CAST_HOSTILE_RANDOM:
        pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
        break;
    case CAST_JUSTDIED_KILLER:
        pTarget = killer;
        break;
    }

    //Target is ok, cast a spell on it
    if (pTarget)
        DoCast(pTarget, Death_Spell);
}

void SimpleAI::UpdateAI(const uint32 diff)
{
    //Return since we have no target
    if (!UpdateVictim())
        return;

    //Spells
    for (uint32 i = 0; i < 10; ++i)
    {
        //Spell not valid
        if (!Spell[i].Enabled || !Spell[i].Spell_Id)
            continue;

        if (Spell_Timer[i] <= diff)
        {
            //Check if this is a percentage based
            if (Spell[i].First_Cast < 0 && Spell[i].First_Cast > -100 && me->GetHealth()*100 / me->GetMaxHealth() > -Spell[i].First_Cast)
                continue;

            //Check Current spell
            if (!(Spell[i].InterruptPreviousCast && me->IsNonMeleeSpellCasted(false)))
            {
                Unit* pTarget = NULL;

                switch (Spell[i].Cast_Target_Type)
                {
                case CAST_SELF:
                    pTarget = me;
                    break;
                case CAST_HOSTILE_TARGET:
                    pTarget = me->getVictim();
                    break;
                case CAST_HOSTILE_SECOND_AGGRO:
                    pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
                    break;
                case CAST_HOSTILE_LAST_AGGRO:
                    pTarget = SelectUnit(SELECT_TARGET_BOTTOMAGGRO,0);
                    break;
                case CAST_HOSTILE_RANDOM:
                    pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
                    break;
                }

                //Target is ok, cast a spell on it and then do our random yell
                if (pTarget)
                {
                    if (me->IsNonMeleeSpellCasted(false))
                        me->InterruptNonMeleeSpells(false);

                    DoCast(pTarget, Spell[i].Spell_Id);

                    //Yell and sound use the same number so that you can make
                    //the creature yell with the correct sound effect attached
                    uint8 random_text = urand(0,2);

                    //Random yell
                    if (Spell[i].TextId[random_text])
                        DoScriptText(Spell[i].TextId[random_text], me, pTarget);

                    //Random sound
                    if (Spell[i].Text_Sound[random_text])
                        DoPlaySoundToSet(me, Spell[i].Text_Sound[random_text]);
                }

            }

            //Spell will cast agian when the cooldown is up
            if (Spell[i].CooldownRandomAddition)
                Spell_Timer[i] = Spell[i].Cooldown + (rand() % Spell[i].CooldownRandomAddition);
            else Spell_Timer[i] = Spell[i].Cooldown;

        } else Spell_Timer[i] -= diff;

    }

    DoMeleeAttackIfReady();
}

