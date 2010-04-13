UPDATE `instance_template` SET `script`='instance_trial_of_the_crussader' WHERE `map`=649;
DELETE FROM `creature` WHERE `map` = 649 AND `id` IN
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);

update creature_template set ScriptName='boss_gormok' where entry='34796';
update creature_template set ScriptName='boss_icehowl' where entry='34797';
update creature_template set ScriptName='npc_dreadscale' where entry='34799';
update creature_template set ScriptName='npc_acidmaw' where entry='35144';
update creature_template set ScriptName='boss_eydis' where entry='34496';
update creature_template set ScriptName='boss_fjola' where entry='34497';
update creature_template set ScriptName='mob_essence_of_twin' where entry='34567';
update creature_template set ScriptName='mob_essence_of_twin' where entry='34568';
update creature_template set ScriptName='mob_power_of_twin' where entry='34628';
update creature_template set ScriptName='mob_power_of_twin' where entry='34630';
update creature_template set ScriptName='boss_lord_jaraksus' where entry='34780';
update creature_template set ScriptName='boss_anub_arak_crusader' where entry='34564';
update creature_template set ScriptName='npc_borrower' where entry='34607';
update creature_template set ScriptName='npc_warrior' where entry='34453';
update creature_template set ScriptName='npc_mage' where entry='34449';
update creature_template set ScriptName='npc_shaman_ench' where entry='34455';
update creature_template set ScriptName='npc_shaman_restor' where entry='34444';
update creature_template set ScriptName='npc_hunter' where entry='34448';
update creature_template set ScriptName='npc_rogue' where entry='34454';
update creature_template set ScriptName='npc_paladin_retro' where entry='34456';
update creature_template set ScriptName='npc_death_knight' where entry='34458';
update creature_template set ScriptName='npc_paladin_holy' where entry='34445';
update creature_template set ScriptName='npc_druid_balans' where entry='34451';
update creature_template set ScriptName='npc_druid_restor' where entry='34459';
update creature_template set ScriptName='npc_priest_disc' where entry='34447';
update creature_template set ScriptName='npc_priest_shadow' where entry='34441';
update creature_template set ScriptName='npc_warlock' where entry='34450';
update creature_template set ScriptName='npc_warrior' where entry='34475';
update creature_template set ScriptName='npc_mage' where entry='34468';
update creature_template set ScriptName='npc_shaman_ench' where entry='34463';
update creature_template set ScriptName='npc_shaman_restor' where entry='34470';
update creature_template set ScriptName='npc_hunter' where entry='34467';
update creature_template set ScriptName='npc_rogue' where entry='34472';
update creature_template set ScriptName='npc_paladin_retro' where entry='34471';
update creature_template set ScriptName='npc_death_knight' where entry='34461';
update creature_template set ScriptName='npc_paladin_holy' where entry='34465';
update creature_template set ScriptName='npc_druid_balans' where entry='34460';
update creature_template set ScriptName='npc_druid_restor' where entry='34469';
update creature_template set ScriptName='npc_priest_disc' where entry='34466';
update creature_template set ScriptName='npc_priest_shadow' where entry='34473';
update creature_template set ScriptName='npc_warlock' where entry='34474';
update creature_template set ScriptName='npc_toc6_announcer' where entry='34816';

UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(12266,12209,12212,12281,12190,12284,12269,12272,12229,12187,12091,12088,12169,12103,12106,12112,12166,12163,12175,12183,12303,12300);
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(12267,12210,12213,12282,12191,12285,12270,12273,12230,12188,12092,12089,12170,12104,12107,12113,12167,12164,12181,12184,12304,12301);
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(12268,12211,12214,12283,12192,12286,12271,12274,12231,12189,12093,12090,12171,12105,12108,12114,12168,12165,12182,12185,12305,12302);

-- N10
DELETE FROM `creature_loot_template` WHERE `entry` IN
(34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);
-- H10
DELETE FROM `creature_loot_template` WHERE `entry` IN
(12266,12209,12212,12281,12190,12284,12269,12272,12229,12187,12091,12088,12169,12103,12106,12112,12166,12163,12175,12183,12303,12300);
-- N25
DELETE FROM `creature_loot_template` WHERE `entry` IN
(12267,12210,12213,12282,12191,12285,12270,12273,12230,12188,12092,12089,12170,12104,12107,12113,12167,12164,12181,12184,12304,12301);
-- H25
DELETE FROM `creature_loot_template` WHERE `entry` IN
(12268,12211,12214,12283,12192,12286,12271,12274,12231,12189,12093,12090,12171,12105,12108,12114,12168,12165,12182,12185,12305,12302);

-- Nortrend beasts
UPDATE creature_template SET ScriptName='boss_gormok_impaler' WHERE entry=34796;
UPDATE creature_template SET ScriptName='boss_acidmaw' WHERE entry=35144;
UPDATE creature_template SET ScriptName='boss_dreadscale' WHERE entry=34799;
UPDATE creature_template SET ScriptName='boss_icehowl' WHERE entry=34797;
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(34796,34799,35144);
DELETE FROM `creature_loot_template`  WHERE  `entry` IN
(34796,34799,35144);

-- Jaraxxus
UPDATE creature_template SET scriptname='boss_lord_jaraxxus' WHERE entry=34780;

-- Anub'arak
UPDATE creature_template SET scriptname='boss_anubarak_trial' WHERE entry=34564;

-- Twin
UPDATE creature_template SET ScriptName='boss_fjola' WHERE entry=34497;
UPDATE creature_template SET ScriptName='boss_eydis' WHERE entry=34496;

DELETE FROM script_texts WHERE entry BETWEEN -1999925 AND -1999939;
INSERT INTO script_texts
    (npc_entry, entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8, sound, type, language, emote, comment)
VALUES
    (34996,-1999926,'Welcome Champions! You heard the call of the Argent Crusade and without hesitation responded to him! In the Crusaders Coliseum, you have to fight with dangerous enemies. Those of you who can survive the test, will enter the ranks of the Argent Crusade, which will go to the IceCrown Citadel ..',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16036,1,0,0,'<need translate>'),
	(34996,-1999927,'Hailing from the deepest, darkest carverns of the storm peaks, Gormok the Impaler! Battle on, heroes!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16038,1,0,0,'<need translate>'),
	(34990,-1713502,'Your monster is no match to the heroes of the Alliance Tirion!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16069,1,0,0,'<need translate>'),
	(34995,-1713702,'Your monster is no match to the heroes of the Horde, Tirion!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16069,1,0,0,'<need translate>'),
	(34996,-1999928,'Steel yourselves heroes, for the twin terrors Acidmaw and Dreadscale enter the arena!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16039,1,0,0,'<need translate>'),
	(34996,-1999929,'The air freezes with the introduction of our next combatant, Icehowl! Kill or be killed, champions!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16040,1,0,0,'<need translate>'),
	(34996,-1999936,'Grand Warlock Wilfred Fizzlebang will summon forth your next challenge. Stand by for his entry!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16043,1,0,0,'<need translate>'),
	(35458,-1999937,'Thank you, Highlord! Now challengers, I will begin the ritual of summoning! When I am done, a fearsome Doomguard will appear!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16268,1,0,0,'<need translate>'),
	(35458,-1999938,'Prepare for oblivion!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16269,1,0,0,'<need translate>'),
	(35458,-1999939,'Ah ha! Behold the absolute power of Wilfred Fizzlebang, master summoner! You are bound to ME, demon!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16270,1,0,0,'<need translate>');