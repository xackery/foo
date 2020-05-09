#event Shrink "[MQ2] shrink"

Sub Event_Shrink
	/if (!${Defined[IsShrinkEnabled]}) /declare IsShrinkEnabled bool outer false
	/varcalc IsShrinkEnabled (!${IsShrinkEnabled})
	/echo Is shrinking enabled? ${IsShrinkEnabled}
/return

Sub Shrink
	/if (!${Defined[IsShrinkEnabled]}) /declare IsShrinkEnabled bool outer false	
	/if (!${IsShrinkEnabled}) /return
	/if (${Zone.ShortName.Equal[kael]}) /return
	/if (${Zone.ShortName.Equal[guildlobby]}) /return
	/if (${Zone.ShortName.Equal[poknowledge]}) /return
	/if (${Zone.ShortName.Equal[guildhall]}) /return
	
	|Dark Elf shrink
	/if (${Me.Race.ID} == 6 && ${Me.Height} > 1.88) {
		/call CastShrink "Ring of the Ancients"
		/call CastShrink "Earring of Diminutiveness"
		/return
	}
	/if (${Me.Race.ID} == 564 && ${Me.Height} > 2.5) {			
		/call CastShrink "Earring of Diminutiveness"
		/return
	}
/return

Sub CastShrink(spellName)
	/if (!${Cast.Ready[${SpellName}]}) /return FALSE
	/casting "${spellName}" -targetid|${Me.ID}
	/delay 1s
	/target clear
/return TRUE