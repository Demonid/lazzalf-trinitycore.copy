-- Hodir Flash Freeze
UPDATE `creature_template` SET `difficulty_entry_1` = 33352, `modelid1` = 25865, `modelid2` = 0, `mechanic_immune_mask` = 612597599, `flags_extra` = 0, `ScriptName` = 'npc_flash_freeze_pre' WHERE `entry` = 32926;
UPDATE `creature_template` SET `difficulty_entry_1` = 33353, `mechanic_immune_mask` = 612597599 WHERE `entry` = 32938;
UPDATE `creature_template` SET `modelid1` = 25865, `modelid2` = 0, `faction_A` = 14, `faction_H` = 14, `unit_flags` = 393220, `mechanic_immune_mask` = 612597599, `flags_extra` = 0 WHERE `entry` IN (33352, 33353);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (61969, 61990);
INSERT INTO `spell_linked_spell` VALUES
(61969, 7940, 2, "Hodir Flash Freeze immunity"),
(61990, 7940, 2, "Hodir Flash Freeze immunity");