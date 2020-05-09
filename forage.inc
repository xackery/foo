
#event Forage "[MQ2] Forage"

Sub Event_Forage
	/if (!${Defined[IsForageEnabled]}) /declare IsForageEnabled bool outer false
	/varcalc IsForageEnabled (!${IsForageEnabled})
	/echo Is foraging enabled? ${IsForageEnabled}
/return

Sub Forage
	/if (!${IsForageEnabled}) /return
	/if (${Me.XTarget}) /return
	/if (${Me.Sitting}) /return
	/if (${Cursor.ID}) /return
	/if (${Me.FreeInventory}<5) /return
	/if (${Me.Casting.ID}) /return
	/if (${Me.Moving}) /return
	/if (${Me.Combat}) /return
	/if (${Me.AutoFire}) /return
	/if (!${Me.AbilityReady[Forage]}) /return
	/if (!${Defined[isForageZone]}) /declare isForageZone bool local
	/varset isForageZone false
	/if (!${Defined[forageZones]}) /declare forageZones string local eastwastestwo|kaeltwo|greatdividetwo|velketortwo|
	/if (!${Defined[forageIndex]}) /declare forageIndex int local

	/for forageIndex 1 to ${forageZones.Count[|]}
		/if (${Zone.ShortName.Lower.NotEqual[${forageZones.Token[${forageIndex},|]}]}) /continue
		/varset isForageZone true
	/next forageIndex	
	/if (!${isForageZone}) /return

	/doability "Forage"
	
	/if (!${Defined[forageBlacklist]}) /declare forageBlacklist string local Fruit|Roots|Rabbit Meat|Pod of Water|Vegetables|Berries|Fishing Grubs|

	/delay 1s ${Cursor.ID}
	/while (${Cursor.ID}) {
		/for forageIndex 1 to ${forageZones.Count[|]}		
			/if (!${Cursor.ID}) /break
			/if (${Cursor.Name.NotEqual[${forageZones.Token[${forageIndex},|]}]}) /continue
			/destroy
			/delay 5 !${Cursor.ID}
		/next forageIndex	

		/if (${Cursor.ID}) {
			/dgtell Got ${Cursor.Name} foraging
			/autoinventory
		}
		/delay 5
	}
/return true