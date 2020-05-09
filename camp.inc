| Camp handles setting a location to return to

#event Camp "[MQ2] Camp"

Sub Event_Camp
	/if (!${Defined[IsCampEnabled]}) /declare IsCampEnabled bool outer false
	/varcalc IsCampEnabled (!${IsCampEnabled})
	/echo Is camping enabled? ${IsCampEnabled}
	/if (${IsCampEnabled}) {
		/varset CampX ${Me.X}
		/varset CampY ${Me.Y}
		/varset CampZ ${Me.Z}
	}
/return


Sub Camp
	/if (!${Defined[IsCampEnabled]}) /declare IsCampEnabled bool outer false
	/if (!${Defined[CampX]}) /declare CampX int outer
	/if (!${Defined[CampY]}) /declare CampY int outer
	/if (!${Defined[CampZ]}) /declare CampZ int outer
	/if (!${Defined[CampRadius]}) /declare CampRadius int outer 20
	/if (${Me.Moving}) /return
	/if (${Me.Combat}) /return
	/if (${Navigation.Active}) /return
	/if (${Me.XTarget}) /return

	/if (!${IsCampEnabled}) /return
	/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:${CampY},${CampX}.${CampZ}]}>${CampRadius}) {
		/nav loc ${CampY} ${CampX} ${CampZ}
		/return
	}
/return