| Merc controls mercenary logic

#event Merc "[MQ2] merc"

Sub Event_Merc
	/if (!${Defined[IsMercEnabled]}) /declare IsMercEnabled bool outer false
	/varcalc IsMercEnabled (!${IsMercEnabled})
	/echo Is merc logic enabled? ${IsMercEnabled}
/return

Sub Merc
	/if (!${Defined[IsMercEnabled]}) /declare IsMercEnabled bool outer true
	/if (!${IsMercEnabled}) /return
	/call mercClericLogic
	/call mercRogueLogic
/return

Sub mercClericLogic
	/if (!${Me.Mercenary.Class.ShortName.Equal[CLR]}) /return
	/if (!${Mercenary.State.Equal[ACTIVE]}) /return
	/if (${Me.MercenaryStance.Equal[PASSIVE]}) /return
	/if (${Me.MercenaryStance.Equal[EFFICIENT]} && (${Me.XTarget}>1 || ${Target.Named})) /stance reactive
	/if (${Me.XTarget}<=1 && !${Target.Named} && ${Me.MercenaryStance.NotEqual[EFFICIENT]}) /stance efficient
/return

Sub mercRogueLogic
	/if (!${Me.Mercenary.Class.ShortName.Equal[ROG]}) /return
	/if (${Me.Combat} && ${Me.MercenaryStance.Equal[PASSIVE]} && ${Target.Type.Equal[NPC]} && ${Target.PctHPs}<95) /stance burn
	/if (!${Me.Combat} && ${Me.MercenaryStance.NotEqual[PASSIVE]}) /stance passive
/return