-- Link dei trash di Freya allo script
UPDATE `creature_template` SET `ScriptName`='mob_freya_trash' WHERE `entry` IN (33430,33431,33525,33526,33527,33528,33355,33354);
-- Update del tempo di respawn dei trash di Freya (2 ore)
UPDATE `creature` SET `spawntimesecs`=2*60*60 WHERE `id` IN (33430,33431,33525,33526,33527,33528,33355,33354);