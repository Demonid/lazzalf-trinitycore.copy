UPDATE `creature_template` SET `unit_flags` = 0, `type_flags` = 0, `ScriptName` = 'boss_hodir' WHERE `entry` = 32845;

UPDATE `creature_template` SET `modelid1` = 11686, `modelid2` = 0 WHERE `entry` IN (34188, 33632, 33802, 34096, 34098);
UPDATE `creature_template` SET `modelid1` = 25865, `modelid2` = 0, `flags_extra` = 0, `ScriptName` = 'mob_flash_freeze' WHERE `entry` = 32938;
UPDATE `creature_template` SET `modelid1` = 28470, `modelid2` = 0, `ScriptName` = 'mob_icicle' WHERE `entry` = 33169;
UPDATE `creature_template` SET `modelid1` = 28470, `modelid2` = 0, `ScriptName` = 'mob_icicle_snowdrift' WHERE `entry` = 33173;
UPDATE `creature_template` SET `modelid1` = 15880, `modelid2` = 0 WHERE `entry` = 33174;
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` = 194173;

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 61990;
INSERT INTO `spell_linked_spell` VALUES 
(61990, -62457, 2, "Ice Shards Immunity (Hodir)"),
(61990, -65370, 2, "Ice Shards Immunity (Hodir)");

UPDATE `creature_template` SET `unit_flags` = 33817094 WHERE `entry` = 33174;
UPDATE `creature_template` SET `mechanic_immune_mask` = 612581215 WHERE `entry` = 32938;
UPDATE `creature_template` SET `unit_flags` = 0 WHERE `entry` = 32941;
UPDATE `creature_template` SET `unit_flags` = 33686016, `flags_extra` = 2 WHERE `entry` = 30298;

DELETE FROM `creature` WHERE `id` = 32938;