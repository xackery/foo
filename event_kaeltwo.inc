#event KaelDuck "#*#The ice encrusted Avatar of War shouts that #1# must bend the knee!#*#"
#event KaelUnduck "#*#The Avatar of War nods, accepting the subservience of those that gave it.#*#"

Sub Event_KaelDuck(line, player)
	/echo player: ${player}
	/if (!${player.Equal[${Me.Name}]}) /return
	/if (${Target.ID}) /squelch /target clear
	/delay 5
	/if (${Me.Ducking}) /return
	/if (${Me.Combat}) /attack	
	/delay 1s
	/keypress num_5
	/dgtell ducking
/return

Sub Event_KaelUnduck(line)
	/if (!${Me.Ducking}) /return	
	/keypress num_5
	/if (!${Me.Combat}) /attack
	/dgtell unducking
/return