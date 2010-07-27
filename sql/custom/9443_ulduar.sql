DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=62475;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
('62475','9032','2','System Shutdown');

UPDATE `gameobject` SET `position_x` = 130.553 WHERE `guid` = 55126;
