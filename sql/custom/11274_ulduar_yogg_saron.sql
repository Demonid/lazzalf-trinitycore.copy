-- Sara
UPDATE `creature_template` SET `unit_flags` = 0, `type_flags` = 0, `mechanic_immune_mask` = 650854235, `ScriptName` = 'boss_sara' WHERE `entry` = 33134;
UPDATE creature_model_info SET bounding_radius = 0.465, combat_reach = 45 WHERE modelid = 29117;
UPDATE `creature` SET `spawndist` = 0 WHERE `id` = 33134;
-- Ominous cloud
DELETE FROM `creature` WHERE `id`=33292;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33947654, `type_flags` = 0, `speed_walk` = 0.2, modelid1 = 28549, modelid2 = 0, `ScriptName` = 'npc_ominous_cloud' WHERE `entry` IN (33292, 32406);
-- Guardian of Yogg-Saron
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `faction_A` = 16, `faction_H` = 16, `mindmg` = 123, `maxdmg` = 123, `attackpower` = 123, `dmg_multiplier` = 123, `baseattacktime` = 1800, `mechanic_immune_mask` = 1234, `ScriptName` = 'npc_guardian_yoggsaron' WHERE `entry` = 33136;
UPDATE creature_model_info SET bounding_radius = 0.62, combat_reach = 1.5 WHERE modelid = 28465;
-- Yogg-Saron
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `mechanic_immune_mask` = 650854235, `ScriptName` = 'boss_yoggsaron' WHERE `entry` = 33288;
UPDATE creature_model_info SET bounding_radius = 0.755, combat_reach = 18 WHERE modelid = 28817;
UPDATE `creature_template` SET  modelid1 = 16946, modelid2 = 0, `minlevel` = 80, `maxlevel` = 80, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33685510, `speed_walk` = 2, `ScriptName` = 'npc_death_orb' WHERE `entry` = 33882;
-- Brain of Yogg-Saron
UPDATE `creature_template` SET `difficulty_entry_1` = 33954, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `mechanic_immune_mask` = 650854235, `ScriptName` = 'boss_brain_yoggsaron' WHERE `entry` = 33890;
UPDATE creature_model_info SET bounding_radius = 0.755, combat_reach = 30 WHERE modelid = 28951;
-- Illusions
UPDATE `creature_template` SET modelid1 = 15880, modelid2 = 0, `minlevel` = 82, `maxlevel` = 82, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33685510, `ScriptName` = 'npc_laughing_skull' WHERE `entry` = 33990;
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `faction_A` = 7, `faction_H` = 7, `mechanic_immune_mask` = 650854235, `ScriptName` = 'npc_illusion' WHERE `entry` IN (33433, 33716, 33717, 33719, 33720, 33567);
UPDATE creature_model_info SET bounding_radius = 0.306, combat_reach = 1.5 WHERE modelid = 28621;
UPDATE creature_model_info SET bounding_radius = 1, combat_reach = 10 WHERE modelid IN (2718, 1687, 2717, 12869);
-- Influence Tentacle
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `faction_A` = 16, `faction_H` = 16, `mindmg` = 123, `maxdmg` = 123, `attackpower` = 123, `dmg_multiplier` = 12 WHERE `entry` = 33943;
UPDATE creature_model_info SET bounding_radius = 0.306, combat_reach = 1.5 WHERE modelid = 28813;
-- Fake npcs
UPDATE `creature_template` SET `unit_flags` = 518, `type_flags` = 0, `flags_extra` = 2 WHERE `entry` IN (33436, 33437, 33536, 33535, 33495, 33523, 33441, 33442);
-- Whispers
UPDATE `script_texts` SET `type` = 4 WHERE `entry` IN (-1603317, -1603340, -1603339);
-- Descend into madness portal
UPDATE `creature_template` SET `npcflag` = 16777216, `unit_flags` = 2, `type_flags` = 0, `ScriptName` = 'npc_descend_into_madness' WHERE `entry` = 34072;
DELETE FROM `creature_template` WHERE entry IN (34122, 34123);
INSERT INTO `creature_template` VALUES
('34122','0','0','0','0','0','29074','0','0','0','Descend Into Madness','','Interact','0','81','81','0','35','35','16777216','1','1.14286','1','0','0','0','0','0','1','2000','0','1','2','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','npc_descend_into_madness','11159'),
('34123','0','0','0','0','0','29074','0','0','0','Descend Into Madness','','Interact','0','81','81','0','35','35','16777216','1','1.14286','1','0','0','0','0','0','1','2000','0','1','2','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','npc_descend_into_madness','11159');
-- Flee to the Surface
UPDATE `gameobject_template` SET `data10` = 63992 WHERE `entry` = 194625;
DELETE FROM `gameobject` WHERE `id` = 194625;
INSERT INTO `gameobject` VALUES
(NULL, 194625, 603, 3, 1, 1996.41, -0.070, 240.59, 0, 0, 0, 1, 0, 300, 0, 1),
(NULL, 194625, 603, 3, 1, 1949.63, -26.07, 241.25, 0, 0, 0, 1, 0, 300, 0, 1),
(NULL, 194625, 603, 3, 1, 1995.03, -52.98, 241.02, 0, 0, 0, 1, 0, 300, 0, 1);
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (34072, 34122, 34123);
INSERT INTO `npc_spellclick_spells` VALUES
(34072, 63989, 0, 0, 0, 3, 0, 0, 0), -- Stormwind
(34122, 63997, 0, 0, 0, 3, 0, 0, 0), -- Chamber
(34123, 63998, 0, 0, 0, 3, 0, 0, 0); -- Icecrown
-- Cancel Illusion Room Aura
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 63992;
INSERT INTO `spell_linked_spell` VALUES
(63992, -63988, 0, "Cancel Illusion Room Aura");
-- Dragon blood
DELETE FROM gameobject WHERE id = 194462;
INSERT INTO gameobject VALUES (NULL, 194462, 603, 3, 1, 2104.35, -25.3753, 242.647, 0, 0, 0, 0, -1, 300, 0, 1);
-- Remove spawned mobs
DELETE FROM `creature` WHERE `id`=34137 AND position_x = 1921.84;
-- Portals coordinates
DELETE FROM `spell_target_position` WHERE `id` IN (63989, 63992, 63997, 63998);
INSERT INTO `spell_target_position` VALUES
(63989, 603, 1953.91, 21.91, 239.71, 2.08),
(63992, 603, 1980.28, -25.59, 329.40, 3.14),
(63997, 603, 2042.02, -25.54, 239.72, 6.28),
(63998, 603, 1948.44, -82.04, 239.99, 4.18);
-- Tentacles
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `type_flags` = 0, `ScriptName` = "npc_constrictor_tentacle" WHERE `entry` = 33983;
UPDATE creature_model_info SET bounding_radius = 0.306, combat_reach = 1.5 WHERE modelid = 28815;
-- Squeeze stun
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (64125, 64126);
INSERT INTO `spell_linked_spell` VALUES
(64125, 9032, 2, "Squeeze stun"),
(64126, 9032, 2, "Squeeze stun");
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 4, `type_flags` = 0, `mindmg` = 123, `maxdmg` = 123, `attackpower` = 123, `dmg_multiplier` = 123, `baseattacktime` = 1800, `ScriptName` = 'npc_crusher_tentacle' WHERE `entry` = 33966;
UPDATE creature_model_info SET bounding_radius = 0.985, combat_reach = 5 WHERE modelid = 28814;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `ScriptName` = 'npc_corruptor_tentacle' WHERE `entry` = 33985;
-- Immortal Guardian
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `mindmg` = 123, `maxdmg` = 132, `attackpower` = 123, `dmg_multiplier` = 12, `type_flags` = 0, `ScriptName` = 'npc_immortal_guardian' WHERE `entry` = 33988;
UPDATE creature_model_info SET bounding_radius = 0.92, combat_reach = 4 WHERE modelid = 29024;
-- Shattered Illusion
-- DELETE FROM `spell_script_target` WHERE `entry` = 64173;
-- INSERT INTO `spell_script_target` VALUES
-- (64173, 1, 33966),
-- (64173, 1, 33983),
-- (64173, 1, 33985);
DELETE FROM `conditions` WHERE `SourceEntry` = 64173;
INSERT INTO `conditions` (SourceTypeOrReferenceId,SourceEntry,ConditionTypeOrReference,ConditionValue1,ConditionValue2) VALUES 
(13, 64173, 18, 1, 33966),
(13, 64173, 18, 1, 33983),
(13, 64173, 18, 1, 33985);
-- Remove area stun
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (64059, 65238);
INSERT INTO `spell_linked_spell` VALUES
(64059, 65238, 0, "Shattered illusion remove"),
(65238, -64173, 1, "Shattered illusion remove");
-- Thorim's Titanic Storm
-- DELETE FROM `spell_script_target` WHERE `entry` = 64172;
-- INSERT INTO `spell_script_target` VALUES
-- (64172, 1, 33988);
DELETE FROM `conditions` WHERE `SourceEntry` = 64172;
INSERT INTO `conditions` (SourceTypeOrReferenceId,SourceEntry,ConditionTypeOrReference,ConditionValue1,ConditionValue2) VALUES 
(13, 64172, 18, 1, 33988);
-- Empowering Shadows
-- DELETE FROM `spell_script_target` WHERE `entry` = 64468;
-- INSERT INTO `spell_script_target` VALUES
-- (64468, 1, 33288),
-- (64468, 1, 33988);
DELETE FROM `conditions` WHERE `SourceEntry` = 64468;
INSERT INTO `conditions` (SourceTypeOrReferenceId,SourceEntry,ConditionTypeOrReference,ConditionValue1,ConditionValue2) VALUES 
(13, 64468, 18, 1, 33288),
(13, 64468, 18, 1, 33988);