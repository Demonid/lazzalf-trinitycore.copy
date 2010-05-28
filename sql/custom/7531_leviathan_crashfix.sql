-- Ulduar crash fixes
UPDATE `creature_template` SET `spell1`=62402 WHERE `entry`=33142;
UPDATE `creature_template` SET `spell1`=62402 WHERE `entry`=33139;
UPDATE `creature_template` SET `spell1`=62402 WHERE `entry`=34111;
UPDATE `creature_template` SET `spell1`=62402 WHERE `entry`=34224;

-- Leviathan Torret 33139
UPDATE `creature_template` SET `minlevel`=80,`maxlevel`=83,`exp`=2,`unit_class`=1 WHERE `entry`=33139;
-- Leviathan Torret 33142
UPDATE `creature_template` SET `minlevel`=80,`maxlevel`=83,`exp`=2,`unit_class`=1 WHERE `entry`=33142;
