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
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(34796,34799,35144);
DELETE FROM `creature_loot_template`  WHERE  `entry` IN
(34796,34799,35144);

UPDATE `creature_template` SET `npcflag`=1, `AIName` ='' WHERE entry = 34568;
UPDATE `creature_template` SET `npcflag`=1, `AIName` ='' WHERE entry = 34567;

UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `modelid1` = 11686, `modelid2` = 11686, `modelid3` = 11686, `modelid4` = 11686, `AIName` ='', `faction_A`= 14, `faction_H` = 14 WHERE entry = 34784;
 
UPDATE `creature_template` SET `modelid1` = 25144, `modelid2` = 0, `modelid3` = 25144, `modelid4` = 0, `faction_A` = 16, `faction_H` = 16, `AIName` = 'EventAI', `ScriptName` = '' WHERE `entry` = 34606;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (34606, 34605, 34607, 34564);
INSERT INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES
(3460651, 34606, 0, 0, 60, 3, 1000, 15000, 60000, 60000, 11, 66193, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 'Frost Sphere - Permafrost'),
(3460654, 34606, 0, 0, 60, 5, 1000, 15000, 60000, 60000, 11, 67855, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 'Frost Sphere - Permafrost'),
(3460652, 34606, 0, 0, 60, 9, 1000, 15000, 60000, 60000, 11, 67856, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 'Frost Sphere - Permafrost'),
(3460653, 34606, 0, 0, 60, 16, 1000, 15000, 60000, 60000, 11, 67857, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 'Frost Sphere - Permafrost');
 


DELETE FROM `script_texts` WHERE `entry` BETWEEN -1713799 AND -1713499;
INSERT INTO `script_texts`(`npc_entry`,`sound`, `entry`,`content_default`,`type`,`language`,`emote`,`comment`) VALUES
('34996','16036','-1713500','Welcome, heroes! You heard the call of the Silver Vanguard and without hesitation responded to him! In the Coliseum, you have to fight with dangerous enemies. Those of you who can survive the test, will enter the ranks of the Silver Vanguard, which will go to the Ice Crown Citadel ..','1','0','0','<need translate>'),
('34996','16038','-1713501','The most profound and dark caves Storm Peaks was designed Gormok Piercing Tusk! In battle, the heroes! Oron ..','1','0','0','<need translate>'),
('34990','16069','-1713502','Your monster is no match to the heroes of the Alliance Tirion!','1','0','0','<need translate>'),
('34995','16069','-1713702','Your monster is no match to the heroes of the Horde, Tirion!','1','0','0','<need translate>'),
('34996','16039','-1713503','Prepare for battle with the twin monsters, acid womb and macabre scales!','1','0','0','<need translate>'),
('34799','0','-1713504','After the death of Comrade% s is furious!','1','0','0','<need translate>'),
('34996','16040','-1713505','The air wafted the icy breath of the following men: the arena goes Ice Roar! Fight or die, the champions!','1','0','0','<need translate>'),
('34797','0','-1713506','% s looks at | 3-3 ($ n) and emits a guttural howl!','1','0','0','<need translate>'),
('34797','0','-1713507','% s crashes into the wall of the Colosseum and lose focus!','1','0','0','<need translate>'),
('34797','0','-1713508','| 3-3 (% s) covers boiling rage, and he trampled all in its path!','1','0','0','<need translate>'),
('34996','16041','-1713509','All the monsters defeated!','1','0','0','<need translate>'),
('34996','16043','-1713510','Now the great magician Wilfred Nepopams call your new enemy. Prepare for battle!','1','0','0','<need translate>'),
('35458','16268','-1713511','Thank you, High Lord. And now, brave, I will proceed to the ritual of Appeal. When I finish, you''ll see a menacing demon!','1','0','0','<need translate>'),
('35458','16269','-1713512','Prepare for oblivion!','1','0','0','<need translate>'),
('35458','16270','-1713513','AHA! Success! Tremble before the all-powerful Wilfried Nepopamsom, master call! You conquered me, demon!','1','0','0','<need translate>'),
('34780','16143','-1713514','Insignificant dwarf! You will ruin your self-confidence!','1','0','0','<need translate>'),
('35458','16271','-1713515','Here I am chief!','1','0','0','<need translate>'),
('35458','16271','-1713715','Well, again, I sucked ...','1','0','0','<need translate>'),
('34996','16044','-1713516','Hurry, heroes, straightened with the lord of demons, before he opens the portal to his kingdom of darkness!','1','0','0','<need translate>'),
('34780','16144','-1713517','Before you Dzharaksus, eredarsky lord of the Burning Legion!','1','0','0','<need translate>'),
('34780','0','-1713518','You have been sent | cFFFF0000Plamya Legion! | R','1','0','0','<need translate>'),
('34780','0','-1713519','% s creates the gates of the Void!','1','0','0','<need translate>'),
('34780','0','-1713520','% s creates | cFF00FF00Vulkan infernals! | r','1','0','0','<need translate>'),
('34780','16150','-1713521','Appear, sister! Lord is calling you!','1','0','0','<need translate>'),
('34780','0','-1713522','$n $g&#1087;&#1086;&#1076;&#1074;&#1077;&#1088;&#1075;&#1089;&#1103;:&#1087;&#1086;&#1076;&#1074;&#1077;&#1088;&#1075;&#1083;&#1072;&#1089;&#1100;; |cFF00FFFF&#1048;&#1089;&#1087;&#1077;&#1087;&#1077;&#1083;&#1077;&#1085;&#1080;&#1102; &#1087;&#1083;&#1086;&#1090;&#1080;!|r &#1048;&#1089;&#1094;&#1077;&#1083;&#1080;&#1090;&#1077; $g&#1077;&#1075;&#1086;:&#1077;&#1077;;!','1','0','0','<need translate>'),
('34780','16149','-1713523','FLESH FROM THE BONES!','1','0','0','<need translate>'),
('34780','16151','-1713524','Infernal!','1','0','0','<need translate>'),
('34780','16147','-1713525','My site will take others. Your world is doomed ...','1','0','0','<need translate>'),
('34996','16045','-1713526','The death of Wilfred Nepopamsa very tragic and should serve as a lesson to those who dare to carelessly play with dark magic. Fortunately, you have conquered the demon, and now awaits a new opponent.','1','0','0','<need translate>'),
('34995','16021','-1713527','Mean Dog Alliance! You released the demon lord of the warriors of the Horde? Your death will be quick!','1','0','0','<need translate>'),
('34990','16064','-1713528','Alliance does not need help with the demon lord to cope with Ordyn breed, dog!','1','0','0','<need translate>'),
('34996','16046','-1713529','Hush! Calm down! No conspiracy here. Warlock played and paid for it. The tournament continues!','1','0','0','<need translate>'),
('34996','16047','-1713530','In the next battle, you will meet with the mighty Knights of the Silver Vanguard! Just beating them, you deserve a decent reward.','1','0','0','<need translate>'),
('34995','16023','-1713531','Horde demands justice! We call the Alliance at the battle! Let us take the place of thy knights, paladins. We show these dogs, as to offend the Horde!','1','0','0','<need translate>'),
('34995','16023','-1713731','Alliance calls for justice! We call the Horde to fight! Let us take the place of thy knights, paladins. We show these dogs, as to offend the Alliance!','1','0','0','<need translate>'),
('34996','16048','-1713532','Ok. So be it. Fight with honor!','1','0','0','<need translate>'),
('34995','16022','-1713533','Do not spare the one, the heroes of the Horde! LTC-TAR Ogar!','1','0','0','<need translate>'),
('34995','16022','-1713733','Do not spare the one, the heroes of the Alliance! For Lordaeron!','1','0','0','<need translate>'),
('34990','16067','-1713534','PRAISE THE ALLIANCE!','1','0','0','<need translate>'),
('34990','16067','-1713734','LTC-TAR Ogar!','1','0','0','<need translate>'),
('34996','16049','-1713535','Empty and bitter victory. After today''s loss, we become weaker as a whole. Who else, besides the Lich King, will benefit from such nonsense? Pali great warriors. And for what? The real danger is still ahead - we are waiting for the battle with the King-lichom.','1','0','0','<need translate>'),
('34996','16050','-1713536','Only a united, you will be able to pass the final test. From the icy depths of the Crown to meet you, rising two mighty warriors of the Scourge: terrible Valkyries, the winged messenger of the Lich King!','1','0','0','<need translate>'),
('34996','16037','-1713537','Let the contest begin!','1','0','0','<need translate>'),
('34497','0','-1713538','%s begins to read a spell | cFFFFFFFFSvetlaya funnel! | r switch to | cFFFFFFFFSvetloy | r essence!','1','0','0','<need translate>'),
('34497','0','-1713539','%s begins to read the spell Treaty twins!','1','0','0','<need translate>'),
('34496','0','-1713540','%s begins to read a spell | cFF9932CDTemnaya funnel! | r switch to | cFF9932CDTemnoy | r essence!','1','0','0','<need translate>'),
('34497','16272','-1713541','In the name of the dark lord. In the name of the Lich King. You. Die.','1','0','0','<need translate>'),
('34496','16279','-1713542','Yes, you will absorb light!','1','0','0','<need translate>'),
('34496','16277','-1713543','Empty place!','1','0','0','<need translate>'),
('34497','16276','-1713544','You appreciated and acknowledged nothing.','1','0','0','<need translate>'),
('34497','16274','-1713545','CHAOS!','1','0','0','<need translate>'),
('34496','16278','-1713546','Yes devour you Dark!','1','0','0','<need translate>'),
('34496','16275','-1713547','Scourge not stop ...','1','0','0','<need translate>'),
('34990','16068','-1713548','Against the Alliance did not survive even the most powerful henchmen of the Lich King! All glorify our heroes!','1','0','0','<need translate>'),
('34995','16068','-1713748','Against the Horde does not withstand even the most powerful henchmen of the Lich King! All glorify our heroes!','1','0','0','<need translate>'),
('34996','16051','-1713549','Lich King suffered a heavy loss! You have shown themselves as fearless heroes of the Silver Vanguard! Together, we will attack on the Citadel Ice Crown and separated into pieces balances the Scourge! There is no test that we could not go together!','1','0','0','<need translate>'),
('16980','16321','-1713550','Will you such a test, Fordring.','1','0','0','<need translate>'),
('34996','16052','-1713551','Arthas! We have much more! Place an Ice grief, and I''ll give you a well-deserved death.','1','0','0','<need translate>'),
('35877','16322','-1713552','Nerubiantsy erected a whole empire under the ice of Northrend. Empire, on which you so thoughtlessly built their homes. MY EMPIRE.','1','0','0','<need translate>'),
('16980','16323','-1713553','The souls of your fallen champions will belong to me, Fordring.','1','0','0','<need translate>'),
('34564','16235','-1713554','And here is the guest said, as he had promised Sir.','1','0','0','<need translate>'),
('34564','16234','-1713555','This place will be your grave!','1','0','0','<need translate>'),
('34564','16240','-1713556','Auum to ac-l-k-k-k, ishshsh. Arise, my servants. Time to devour ...','1','0','0','<need translate>'),
('34564','0','-1713557','%s buries itself in the ground!','1','0','0','<need translate>'),
('34660','0','-1713558','Spikes | 3-1 (% s) pursuing | 3-3 ($ n)!','1','0','0','<need translate>'),
('34564','0','-1713559','%s comes out of the ground!','1','0','0','<need translate>'),
('34564','16241','-1713560','Flock swallow you!','1','0','0','<need translate>'),
('34564','0','-1713561','%s produces a swarm of beetles Necrocannibals to restore your health!','1','0','0','<need translate>'),
('34564','16236','-1713562','F-lakkh w!','1','0','0','<need translate>'),
('34564','16237','-1713563','Another feed the soul master.','1','0','0','<need translate>'),
('34564','16238','-1713564','I''ve failed you, sir ...','1','0','0','<need translate>'),
('36095','0','-1713565','Champions, you have passed the test of the great crusader! Be aware that only the strongest adventurers can expect to complete this test.','1','0','0','<need translate>'),
('36095','0','-1713566','Let me give you this chest as a well-deserved reward, and let its contents will serve you faithfully in the campaign against Arthas in the heart of the Ice Crown Citadel!','1','0','0','<need translate>');