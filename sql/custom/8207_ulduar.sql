DELETE FROM `creature_ai_scripts` WHERE (`creature_id`=33768);
INSERT INTO `creature_ai_scripts` VALUES
( 3376801, 33768, 0, 0, 100, 3, 6000, 12000, 8000, 16000, 11, 63978, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Rubble (Normal) - Cast Stone Nova'),
( 3376802, 33768, 0, 0, 100, 5, 6000, 12000, 8000, 16000, 11, 63978, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Rubble (Heroic) - Cast Stone Nova');

DELETE FROM spell_script_target WHERE entry = 62343;
INSERT INTO spell_script_target VALUES (62343, 1, 33121);

UPDATE `creature_template` SET `unit_flags` = 33947654 WHERE `entry` = 33121;