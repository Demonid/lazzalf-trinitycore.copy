DELETE FROM `creature_template` WHERE `entry` IN (33119);

INSERT INTO `creature_template` VALUES
('33119','0','0','0','0','0','13069','0','0','0','Scorch trigger','',NULL,'0','83','83','2','16','16','0','2','85','3','0','0','0','0','0','0','0','8','34340934','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','62548','0','0','0','0','0','0','0','0','0','1305000','1595000','EventAI','0','3','310','30','1','0','0','0','0','0','0','0','169','1','0','617299803','1','trigger_periodic','0');

DELETE FROM `creature_ai_scripts` WHERE (`creature_id`=33119);
INSERT INTO `creature_ai_scripts` VALUES 
( 10033119, 33119, 0, 0, 100, 3, 1000, 1000, 1000, 1000, 11, 62548, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Scorch Ground');

UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `unit_flags` = 33554434, `ScriptName` = 'mob_iron_construct', `flags_extra` = 0 WHERE `entry` = 33121;