Sub Logic
	/if (!${Defined[ProgressionTimer]}) /declare ProgressionTimer timer outer
	/if (${Me.Combat} && ${Target.ID} == ${Me.ID}) {
		/echo turning off attack (targetting self)
		/attack
	}
	/if (${IsSafeZone[]}) /return
	/if (!${Defined[IsAssistAllowed]}) /declare IsAssistAllowed bool outer true
	
	/call Event
	/call Loot
	/call Camp
	/call Forage
	
	/call AssistTarget
	/call Engage
	
	/call Merc
	/call Shrink
/return