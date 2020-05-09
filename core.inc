| Core file, used for a lot of essential includes

#event CannotCast "You cannot cast #1# on #2#."
#event Help "[MQ2] help"

#include foo/augac.inc
#include foo/ber.inc
#include foo/brd.inc
#include foo/burn.inc
#include foo/camp.inc
#include foo/clr.inc
#include foo/count.inc
#include foo/engage.inc
#include foo/event.inc
#include foo/event_kaeltwo.inc
#include foo/face.inc
#include foo/forage.inc
#include foo/init.inc
#include foo/kill.inc
#include foo/logic.inc
#include foo/loot.inc
#include foo/merc.inc
|excluded, if you have progg you can enable
|#include foo/progression.inc
|excluded, if you have progg you can enable
|#include foo/travel.inc
#include foo/shrink.inc

sub Event_Help
	/echo /echo burn: set target as burn if you're main assist
	/echo /echo augac: scans all players and reports their type 7 augs
	/echo /echo group: invites players in zone you control to your group
	/echo /echo camp: toggles a camp location to return to when battle ends (${IsCampEnabled})
	/echo /echo count: cursor and advloot, counts everyone in crew their count of the item
	/echo /echo engage: toggles engaging mobs (melee/stick them) (${IsEngageEnabled})
	/echo /echo face: toggles facing an enemy every 6s (${IsFaceEnabled})
	/echo /echo forage: toggles foraging for items (${IsForageEnabled})
	/echo /echo kill [npc names|delimited|]: kill npc names provided with | delimiter (${KillTarget.Length})
	/echo /echo loot [commands]: type /echo loot for details
	/echo /echo merc: toggle merc stance changing logic	(${IsMercEnabled})
	/echo /echo shrink: toggle shrink (${IsShrinkEnabled})
	/call ${Me.Class.ShortName}Help
/return

sub IsInGroup(string classShortName)
	/declare focus string local
	/declare i int local
	/for i 0 to ${Me.GroupList.Length}
		/varset focus Group.Member[${i}]
		/if (!${${focus}.Class.ShortName.Equal[${${classShortName}}]}) /continue
		/if (!${${focus}.Present}) /continue
		/if (${${focus}.Mercenary}) /continue
		/if (${${focus}.Offline}) /continue
		/if (${${focus}.OtherZone}) /continue
		/if (${${focus}.Type.Equal[Corpse]}) /continue	
		/return true
	/next i
/return false

sub IsPlayerInGroup(string playerName) 
	/declare i int local
	/for i 0 to ${Me.GroupList.Length}
		/if (!${Group.Member[${i}].Name.Equal[${playerName}]}) /continue
		/if (!${Group.Member[${i}].Present}) /continue
		/if (${Group.Member[${i}].Offline}) /continue
		/if (${Group.Member[${i}].OtherZone}) /continue
		/return true
	/next i
/return false

sub IsMainTank
	/if (${Group.MainTank.ID}==${Me.ID}) /return true
/return false

sub IsSpellMemorized(string spellName)
	/declare i int local
	/for i 1 to 13
		/if (!${Me.Gem[${i}].Name.Equal[${spellName}]}) /continue
		/return true
	/next i
/return false

sub NearbyXTargets
	/declare i int local
	/declare xCount int local 0
	/for i 1 to ${Me.XTarget}
		/if (!${Me.XTarget[${i}].Type.Equal[NPC]}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Group Assist Target]}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Raid Assist 1 Target]}) /continue
		/if (${Me.XTarget[${i}].Distance} > 50) /continue
		/if (${Me.XTarget[${i}].Moving}) /continue
		/if (${Me.XTarget[${i}].PctHPs}<1) /continue
		/varcalc xCount ${xCount}+1
	/next i
/return ${xCount}

sub IsSafeZone
	/if (${Zone.ShortName.Equal[poknowledge]}) /return true
	/if (${Zone.ShortName.Equal[GuildLobby]}) /return true
/return false

sub IsNamed(int targetID)	
	/if (${Spawn[id ${targetID}].CleanName.Equal[Di`Zok royal guard]}) /return false
	/if (${Spawn[id ${targetID}].CleanName.Equal[Arisen bones]}) /return false
	/if (${Spawn[id ${targetID}].CleanName.Equal[Dronan bodyshield]}) /return false
	/if (${Spawn[id ${targetID}].Named}) /return true
/return false

sub IsMainTank
	/if (${Group.MainTank.ID} == ${Me.ID}) /return true
	/if (${Raid.MainAssist} && ${Raid.MainAssist.ID} == ${Me.ID}) /return true
/return false

sub PetTauntCheck
	/if (!${Pet.ID}) /return
	/declare isTaunt bool local

	/if (${Target.CleanName.Equal[Platinum Rainbow Spire]}) /varset isTaunt true
	/if (${Target.CleanName.Equal[Nureya Arisen]}) /varset isTaunt true

	/if (${isTaunt} && ${Pet.Taunt}) /return
	/if (!${isTaunt} && !${Pet.Taunt}) /return
	/pet taunt
/return

sub IsPrimaryAggro
	/declare i int local
	/for i 1 to ${Me.XTarget}
		/if (!${Me.XTarget[${i}].Type.Equal[NPC]}) /continue
		/if (!${Me.XTarget[${i}].PctAggro}>=100) /return true
	/next i
/return false

Sub NearestLowHPXTarget
	/if (${Me.XTarget}==0) /return 0
	/declare closest int local
	/declare lowestHP int local 100
	/declare i int local
	/declare isNamed bool local FALSE

	/for i 1 to ${Me.XTarget}
		/if (!${Me.XTarget[${i}].Type.Equal[NPC]}) /continue
		/if (${closest}==0) {			
			
			/varcalc closest ${Me.XTarget[${i}].ID}
			/varset lowestHP ${Me.XTarget[${i}].PctHPs}
		}
		
		/if (${isNamed}) /continue
		/if (!${IsValidNPC[${Me.XTarget[${i}].ID}]}) /continue
		/if (${Me.XTarget[${i}].CleanName.Find['s corpse]}) /continue
		/if (${Me.XTarget[${i}].PctHPs}<1) /continue
		/if (${Me.XTarget[${i}].Named} && ${Me.XTarget[${i}].Distance}<80 && ${Target.LineOfSight}) {
			/varset isNamed TRUE
			/varcalc closest ${Me.XTarget[${i}].ID}
			/varset lowestHP ${Me.XTarget[${i}].PctHPs}
			/continue
		}
		/if (${Me.XTarget[${i}].PctHPs}<${lowestHP} && ${Me.XTarget[${i}].Distance}<100) {
			/varcalc closest ${Me.XTarget[${i}].ID}
			/varset lowestHP ${Me.XTarget[${i}].PctHPs}
			/continue
		}
		/if (${Me.XTarget[${i}].Distance} < ${Spawn[id ${closest}].Distance}) {
			/varcalc closest ${Me.XTarget[${i}].ID}
			/varset lowestHP ${Me.XTarget[${i}].PctHPs}
			/continue
		}
	/next i
	/if (${Spawn[id ${closest}].Distance}>50) /return 0
/return ${closest}

Sub IsInCombat
	/if (${Me.CombatState.Equal[COMBAT]}) /return true
/return false

Sub FindSpell(string name)
	/if (!${Defined[newName]}) /declare newName string local
	/varset newName ${name}

	/if (${Me.AltAbility[${newName}]}) /return ${newName}
	/if (${Me.Book[${newName}]}) /return ${newName}
	/if (${Me.CombatAbility[${newName}]}) /return ${newName}
	/varset newName ${name} Rk. II
	/if (${Me.Book[${name}]}) /return ${newName}
	/if (${Me.CombatAbility[${newName}]}) /return ${newName}
	/varset newName ${name} Rk. III
	/if (${Me.Book[${newName}]}) /return ${newName}
	/if (${Me.CombatAbility[${newName}]}) /return ${newName}
/return NULL

Sub IsOutOfCombat
	/if (${Me.CombatState.Equal[ACTIVE]}) /return false
	/if (${Me.CombatState.Equal[COMBAT]}) /return false
/return true

Sub CastMem(string spellName)
	/if (!${Target.ID}) /return
	/if (!${Cast.Ready[${spellName}]}) {
		/memorize "${spellName}" gem13
:WaitForMem1
		/delay 1s
		/if (${Cast.Status.Equal[M]}) /goto :WaitForMem1
	}
	/delay 1s

	/casting "${spellName}" gem13 -maxtries|3 -invis
	/dgtell Buff ${spellName} -> ${Target.Name}
:WaitForCast1
	/delay 1s
	/if (${Cast.Status.Equal[C]}) /goto :WaitForCast1
	/delay 1s
/return

sub XTargetHighestLevel
	/declare i int local
	/declare level int local 0
	/for i 1 to ${Me.XTarget}
		/if (!${Me.XTarget[${i}].Type.Equal[NPC]}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Group Assist Target]}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Raid Assist 1 Target]}) /continue
		/varset level ${Me.XTarget[${i}].Level}
	/next i
/return ${level}

Sub NearestXTarget
	/if (${Me.XTarget}==0) /return 0
	/declare closest int local
	/declare i int local
	/declare isNamed bool local FALSE

	/for i 1 to ${Me.XTarget}		
		/if (${closest}==0) /varcalc closest ${Me.XTarget[${i}].ID}
		/if (${isNamed}) /continue
		/if (${Me.XTarget[${i}].CleanName.Find['s corpse]}) /continue
		/if (!${Me.XTarget[${i}].Type.Equal[NPC]}) /continue
		/if (!${IsValidNPC[${Me.XTarget[${i}].CleanName}]}) /continue

		/if (${Me.XTarget[${i}].Named} && ${Me.XTarget[${i}].Distance}<80 && ${Target.LineOfSight}) {
			/varset isNamed TRUE
			/varcalc closest ${Me.XTarget[${i}].ID}
			/continue
		}
		/if (${Me.XTarget[${i}].Distance} < ${Spawn[id ${closest}].Distance}) /varcalc closest ${Me.XTarget[${i}].ID}
	/next i
	/if (${Spawn[id ${closest}].Distance}>50) /return 0
/return ${closest}

Sub IsXTarget(targetID)
	/declare i int local
	/for i 1 to ${Me.XTarget}
		/if (${Me.XTarget[${i}].ID}==${targetID}) /return TRUE
	/next i
/return FALSE

Sub NearestXTarget
	/if (${Me.XTarget}==0) /return 0
	/declare closest int local
	/declare i int local
	/declare isNamed bool local FALSE

	/for i 1 to ${Me.XTarget}		
		/if (${closest}==0) /varcalc closest ${Me.XTarget[${i}].ID}
		/if (${isNamed}) /continue
		/if (${Me.XTarget[${i}].CleanName.Find['s corpse]}) /continue
		/if (!${Me.XTarget[${i}].Type.Equal[NPC]}) /continue
		/if (!${IsValidNPC[${Me.XTarget[${i}].CleanName}]}) /continue

		/if (${Me.XTarget[${i}].Named} && ${Me.XTarget[${i}].Distance}<80 && ${Target.LineOfSight}) {
			/varset isNamed TRUE
			/varcalc closest ${Me.XTarget[${i}].ID}
			/continue
		}
		/if (${Me.XTarget[${i}].Distance} < ${Spawn[id ${closest}].Distance}) /varcalc closest ${Me.XTarget[${i}].ID}
	/next i
	/if (${Spawn[id ${closest}].Distance}>50) /return 0
/return ${closest}

Sub HasBuff(buffName)
	/if (${Target.ID}==0) /return TRUE
	/if (${Bool[${Target.Buff[${buffName}]}]}) {
		/if (${Target.Buff[${buffName}].ID}>0) /return TRUE
		/if (!${Target.Buff[${buffName}].Stacks}) /return TRUE
	}
/return FALSE

Sub IsCaster
	/if (${Target.Class.ShortName.Equal[WIZ]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[MAG]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[NEC]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[ENC]}) /return TRUE
/return FALSE

Sub IsMelee
	/if (${Target.Class.ShortName.Equal[ROG]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[BER]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[SHD]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[BRD]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[PAL]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[WAR]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[MNK]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[BST]}) /return TRUE
/return FALSE

Sub IsTank
	/if (${Target.Class.ShortName.Equal[SHD]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[PAL]}) /return TRUE
	/if (${Target.Class.ShortName.Equal[WAR]}) /return TRUE	
/return FALSE

Sub IsOKToCast(npc)
	/if (${Ini[immune_npcs.ini,${Zone.ShortName},"${npc}"].Equal[1]}) /return FALSE
/return TRUE

Sub Event_CannotCast(line, spell, npc)
	/if (!${Ini[immune_npcs.ini,${Zone.ShortName},${npc}].Equal[1]}) {
		/ini "immune_npcs.ini" "${Zone.ShortName}" "${npc}" "1"
		/echo Added ${npc} to immune list
	}
	/echo clearing target cannotcast
	/target clear
/return

Sub IsLongBuffOK
	/if (${Me.Moving}) /return false
	/if (${NearbyXTargets[]}>0) /return false
	/if (${Window[CastingWindow]}) /return false
	/if (${Me.SpellInCooldown}) /return false
	/if (${Me.PctMana}<50) /return false
	/if (${Me.Invis}) /return false
/return true

Sub GroupBuff(spellName, buffName)
	/if (!${IsLongBuffOK[]}) /return
	
	/if (!${Defined[dout]}) /declare dout string local
	/if (!${Defined[gi]}) /declare gi int local
	/if (!${Defined[gName]}) /declare gName string local
	/if (!${Defined[gID]}) /declare gID int local

	/for gi 0 to ${Group.Members}
		/if (!${DanNet.Peers.Find[${Group.Member[${gi}].Name.Lower}|]}) /continue
		
		/if (!${Group.Member[${gi}].Present}) /continue
		/if (${Group.Member[${gi}].Mercenary}) /continue		
		/if (${Group.Member[${gi}].Offline}) /continue			
		/if (${Group.Member[${gi}].OtherZone}) /continue		
		/if (${Group.Member[${gi}].Distance}>200) /continue
		/if (${Group.Member[${gi}].Type.Equal[Corpse]}) /continue
		/if (!${Group.Member[${gi}].LineOfSight}) /continue

		/varset gName ${Group.Member[${gi}].Name}
		/varset gID ${Group.Member[${gi}].ID}
	
		/dquery ${gName} -q "Me.Buff[${buffName}].ID" -o dout		
		/if (${dout} != NULL) /continue
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/memorize "${spellName}" gem13
		/delay 10s ${Cast.Status.NotEqual[M]}
		/casting "${spellName}" gem13 -targetid|${gID} -maxtries|3
		/echo GroupBuff ${spellName} -> ${gName}
		/return
	/next gi
/return



Sub TankBuff(spellName, buffName)
	/if (!${IsLongBuffOK[]}) /return
	
	/if (!${Defined[dout]}) /declare dout string local
	/if (!${Defined[gi]}) /declare gi int local
	/if (!${Defined[gName]}) /declare gName string local
	/if (!${Defined[gID]}) /declare gID int local

	/for gi 0 to ${Group.Members}
		/if (!${DanNet.Peers.Find[${Group.Member[${gi}].Name.Lower}|]}) /continue
		/if (${Group.MainTank.ID} != ${Group.Member[${gi}].ID}) /continue
		/if (!${Group.Member[${gi}].Present}) /continue
		/if (${Group.Member[${gi}].Mercenary}) /continue		
		/if (${Group.Member[${gi}].Offline}) /continue
		/if (${Group.Member[${gi}].OtherZone}) /continue		
		/if (${Group.Member[${gi}].Distance}>200) /continue
		/if (${Group.Member[${gi}].Type.Equal[Corpse]}) /continue
		/if (!${Group.Member[${gi}].LineOfSight}) /continue
		

		/varset gName ${Group.Member[${gi}].Name}
		/varset gID ${Group.Member[${gi}].ID}

	
		/dquery ${gName} -q "Me.Buff[${buffName}].ID" -o dout		
		/if (${dout} != NULL) /continue
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/memorize "${spellName}" gem13
		/delay 10s ${Cast.Status.NotEqual[M]}
		/casting "${spellName}" gem13 -targetid|${gID} -maxtries|3
		/echo TankBuff ${spellName} -> ${gName}
		/return
	/next gi
/return


Sub IsPluginEnabled(string name)
/return ${Bool[${Plugin[${name}]}]}
