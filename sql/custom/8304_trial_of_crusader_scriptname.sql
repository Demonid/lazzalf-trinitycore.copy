UPDATE `instance_template` SET `script`='instance_trial_of_the_crusader' WHERE `map`=649;
UPDATE `creature_template` SET `npcflag`=1, `scriptname`='npc_toc6_announcer' WHERE `entry`=34816;

-- grand crusaders alliance
UPDATE `creature_template` SET `scriptname`='npc_death_knight' WHERE `entry`=34461;
UPDATE `creature_template` SET `scriptname`='npc_druid_balans' WHERE `entry`=34460;
UPDATE `creature_template` SET `scriptname`='npc_druid_restorr' WHERE `entry`=34469;
UPDATE `creature_template` SET `scriptname`='npc_hunter' WHERE `entry`=34467;
UPDATE `creature_template` SET `scriptname`='npc_mage' WHERE `entry`=34468;
UPDATE `creature_template` SET `scriptname`='npc_paladin_holy' WHERE `entry`=34465;
UPDATE `creature_template` SET `scriptname`='npc_paladin_retro' WHERE `entry`=34471;
UPDATE `creature_template` SET `scriptname`='npc_priest_disc' WHERE `entry`=34466;
UPDATE `creature_template` SET `scriptname`='npc_priest_shadow' WHERE `entry`=34473;
UPDATE `creature_template` SET `scriptname`='npc_rogue' WHERE `entry`=34472;
UPDATE `creature_template` SET `scriptname`='npc_shaman_ench' WHERE `entry`=34463;
UPDATE `creature_template` SET `scriptname`='npc_warlock' WHERE `entry`=34474;
UPDATE `creature_template` SET `scriptname`='npc_warrior' WHERE `entry`=34475;

-- grand crusaders horde
UPDATE `creature_template` SET `scriptname`='npc_death_knight' WHERE `entry`=34458;
UPDATE `creature_template` SET `scriptname`='npc_druid_balans' WHERE `entry`=34451;
UPDATE `creature_template` SET `scriptname`='npc_druid_restor' WHERE `entry`=34459;
UPDATE `creature_template` SET `scriptname`='npc_hunter' WHERE `entry`=34448;
UPDATE `creature_template` SET `scriptname`='npc_mage' WHERE `entry`=34449;
UPDATE `creature_template` SET `scriptname`='npc_paladin_holy' WHERE `entry`=34445;
UPDATE `creature_template` SET `scriptname`='npc_paladin_retro' WHERE `entry`=34456;
UPDATE `creature_template` SET `scriptname`='npc_priest_disc' WHERE `entry`=34447;
UPDATE `creature_template` SET `scriptname`='npc_priest_shadow' WHERE `entry`=34441;
UPDATE `creature_template` SET `scriptname`='npc_rogue' WHERE `entry`=34454;
UPDATE `creature_template` SET `scriptname`='npc_shaman_ench' WHERE `entry`=34455;
UPDATE `creature_template` SET `scriptname`='npc_warlock' WHERE `entry`=34450;
UPDATE `creature_template` SET `scriptname`='npc_warrior' WHERE `entry`=34453;

-- Nortrend beasts
UPDATE creature_template SET ScriptName='boss_gormok' WHERE entry=34796;
UPDATE creature_template SET ScriptName='npc_acidmaw' WHERE entry=35144;
UPDATE creature_template SET ScriptName='npc_dreadscale' WHERE entry=34799;
UPDATE creature_template SET ScriptName='boss_icehowl' WHERE entry=34797;

-- Lord Jaraxxus
UPDATE creature_template SET ScriptName='boss_lord_jaraksus' WHERE entry=34780;

-- Anub'arak
UPDATE creature_template SET scriptname='boss_anub_arak_crusader' WHERE entry=34564;
UPDATE creature_template SET scriptname='npc_borrower' WHERE entry=34607;

-- Twin
UPDATE creature_template SET ScriptName='boss_fjola' WHERE entry=34497;
UPDATE creature_template SET ScriptName='boss_eydis' WHERE entry=34496;
UPDATE creature_template SET ScriptName='mob_essence_of_twin' WHERE entry=34568;
UPDATE creature_template SET ScriptName='mob_essence_of_twin' WHERE entry=34567;
UPDATE creature_template SET ScriptName='mob_power_of_twin' WHERE entry=34630;
UPDATE creature_template SET ScriptName='mob_power_of_twin' WHERE entry=34628;
