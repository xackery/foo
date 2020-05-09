#event Group "[MQ2] group"

Sub Event_Group
	/if (!${Defined[crewIndex]}) /declare crewIndex int local
	/if (!${Defined[cn]}) /declare cn string local

	/for crewIndex 1 to ${Math.Calc[${DanNet.Peers.Count[|]}]}
		/varset cn ${DanNet.Peers.Token[${crewIndex},|]}
		/if (${IsPlayerInGroup[${cn}]}) /continue
		/if (!${Spawn[pc ${cn}].ID}) /continue
		/invite ${cn}
	/next crewIndex
	
	/delay 1s
	/dgzexecute /invite
	/delay 1s
	/grouproles set ${Me.Name} 1
	/grouproles set ${Me.Name} 2
	/grouproles set ${Me.Name} 4
	/grouproles set ${Me.Name} 5
/return false