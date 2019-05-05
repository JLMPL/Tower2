
function initializeLevel()
    setLevelMesh("empty.obj")
    setPlayerSpawnPoint(-2,2,0)

    local skello = addCreature(Species_Skeleton, 2,0,0)

    local chesty = addChest(3,0,0)
    -- addItemToChest(chesty, 0)

    local dorr = addDoor(-3,2,0)

end
