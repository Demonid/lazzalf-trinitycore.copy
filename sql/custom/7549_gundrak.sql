-- Set correct faction for Slad'ran Viper (Normal,Heroic)
    update creature_template set faction_A=16, faction_H=16 where entry in (29680,30940);
-- Set correct faction for Slad'ran Constrictor (Normal,Heroic)
    update creature_template set faction_A=16, faction_H=16 where entry in (29713,30943);
-- Set correct level and health for Rhino Spirit (Normal)
    --  update creature_template set minlevel=77, maxlevel=77, minhealth=294250, maxhealth=294250 where entry=29791;
    update creature_template set minlevel=77, maxlevel=77 where entry=29791;
-- Set correct faction for Drakkari Elementa (Normal,Heroic)
    update creature_template set faction_A=16, faction_H=16 where entry in (29573,31367);
-- Set correct faction for Eck (Heroic)
    update creature_template set faction_A=16, faction_H=16 where entry =29932;
-- Add scipt name for Ruins Dweller.
    update creature_template set ScriptName = 'npc_ruins_dweller' where entry =29920;