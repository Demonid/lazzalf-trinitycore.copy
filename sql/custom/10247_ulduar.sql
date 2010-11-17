-- XT-002 vehicle id
UPDATE `creature_template` SET `unit_flags` = 33554432, `type_flags` = 0, `VehicleId` = 335 WHERE `entry` = 33293;
-- Despawn Iron Construct (Ignis)
DELETE FROM creature WHERE id = 33121;