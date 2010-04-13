UPDATE `instance_template` SET `script`='instance_trial_of_the_crussader' WHERE `map`=649;
DELETE FROM `creature` WHERE `map` = 649 AND `id` IN
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);

-- announcers
UPDATE `creature_template` SET `npcflag`=1, `scriptname`='npc_tcrus_announcer' WHERE `entry`=34816;
UPDATE `creature_template` SET `scriptname`='npc_tcrus_fizzlebang' WHERE `entry`=35458;
UPDATE `creature_template` SET `scriptname`='npc_tcrus_tirion' WHERE `entry`=34996;

-- grand crusaders alliance
UPDATE `creature_template` SET `scriptname`='boss_tyrius_duskblade' WHERE `entry`=34461;
UPDATE `creature_template` SET `scriptname`='boss_kavina_grovesong' WHERE `entry`=34460;
UPDATE `creature_template` SET `scriptname`='boss_melador_valestrider' WHERE `entry`=34469;
UPDATE `creature_template` SET `scriptname`='boss_alyssia_moonstalker' WHERE `entry`=34467;
UPDATE `creature_template` SET `scriptname`='boss_noozle_whizzlestick' WHERE `entry`=34468;
UPDATE `creature_template` SET `scriptname`='boss_velanaa' WHERE `entry`=34465;
UPDATE `creature_template` SET `scriptname`='boss_baelnor_lightbearer' WHERE `entry`=34471;
UPDATE `creature_template` SET `scriptname`='boss_anthar_forgemender' WHERE `entry`=34466;
UPDATE `creature_template` SET `scriptname`='boss_brienna_nightfell' WHERE `entry`=34473;
UPDATE `creature_template` SET `scriptname`='boss_irieth_shadowstepe' WHERE `entry`=34472;
UPDATE `creature_template` SET `scriptname`='boss_shaabad' WHERE `entry`=34463;
UPDATE `creature_template` SET `scriptname`='boss_saamul' WHERE `entry`=34470;
UPDATE `creature_template` SET `scriptname`='boss_serissa_grimdabbler' WHERE `entry`=34474;
UPDATE `creature_template` SET `scriptname`='boss_shocuul' WHERE `entry`=34475;

-- grand crusaders horde
UPDATE `creature_template` SET `scriptname`='boss_gorgrim_shadowcleave' WHERE `entry`=34458;
UPDATE `creature_template` SET `scriptname`='boss_birana_stormhoof' WHERE `entry`=34451;
UPDATE `creature_template` SET `scriptname`='boss_erin_misthoof' WHERE `entry`=34459;
UPDATE `creature_template` SET `scriptname`='boss_rujkah' WHERE `entry`=34448;
UPDATE `creature_template` SET `scriptname`='boss_ginselle_blightslinger' WHERE `entry`=34449;
UPDATE `creature_template` SET `scriptname`='boss_liandra_suncaller' WHERE `entry`=34445;
UPDATE `creature_template` SET `scriptname`='boss_malithas_brightblader' WHERE `entry`=34456;
UPDATE `creature_template` SET `scriptname`='boss_caiphus_the_stern' WHERE `entry`=34447;
UPDATE `creature_template` SET `scriptname`='boss_vivienne_blackwhisper' WHERE `entry`=34441;
UPDATE `creature_template` SET `scriptname`='boss_mazdinah' WHERE `entry`=34454;
UPDATE `creature_template` SET `scriptname`='boss_broln_stouthorn' WHERE `entry`=34455;
UPDATE `creature_template` SET `scriptname`='boss_thrakgar' WHERE `entry`=34444;
UPDATE `creature_template` SET `scriptname`='boss_harkzog' WHERE `entry`=34450;
UPDATE `creature_template` SET `scriptname`='boss_narrhok_steelbreaker' WHERE `entry`=34453;


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