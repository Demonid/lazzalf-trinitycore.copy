-- Tesla Coil
UPDATE `creature_template` SET `unit_flags` = 33685766 WHERE `entry` = 16218;

-- Feugen e Stalagg speed
UPDATE `creature_template` SET `speed` = 0.3 WHERE `entry` IN (15930, 29447, 15929, 29446);