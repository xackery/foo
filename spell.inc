#Event CannotCast "You cannot cast #1# on #2#."

Sub HasBuff(buffName)
	/if (${Target.ID}==0) /return TRUE
	/if (${Bool[${Target.Buff[${buffName}]}]}) {
		/if (${Target.Buff[${buffName}].ID}>0) /return TRUE
		/if (!${Target.Buff[${buffName}].Stacks}) /return TRUE
	}
/return FALSE

Sub HasSong(buffName)
/if (!${Bool[${Target.Buff[Spirit of the Stoic One Rk. II]}]}) /return FALSE
/return TRUE


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