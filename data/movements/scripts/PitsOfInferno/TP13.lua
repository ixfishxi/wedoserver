--function by Armageddom--
function onStepIn(cid, item, frompos, item2, topos) 

 
local novapos = {x=458, y=1376, z=8} 

if item.uid == 10238 then

getThingfromPos(getPlayerPosition(cid)) 
doSendMagicEffect(getPlayerPosition(cid),2) 
doTeleportThing(cid,novapos) 
doSendMagicEffect(novapos,10) 
end 
end