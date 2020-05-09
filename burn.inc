#event Burn "[MQ2] burn"

Sub Event_Burn
	/if (!${Defined[BurnID]}) /declare BurnID int outer
	/varset BurnID ${Target.ID}
	/echo starting burn on ${Target.Name} ${Target.ID}
/return


Sub Burn	
	/if (!${Defined[BurnTimer]}) /declare BurnTimer timer outer
	/if (!${Defined[BurnID]}) /declare BurnID int outer
	/if (!${Defined[IsBurnQueryEnabled]}) /declare IsBurnQueryEnabled bool outer
	/if (!${Defined[BurnAssistName]}) /declare BurnAssistName string outer
	
	/if (!${Defined[burnTempID]}) /declare burnTempID int local

	/if (${Group} && ${Group.MainAssist.ID} &&  !${Group.MainAssist.Name.Equal[${BurnAssistName}]}) {
		/varset BurnAssistName ${Group.MainAssist.Name}
		/if (${Me.Name.Equal[${Group.MainAssist.Name}]}) {
			/varset IsBurnQueryEnabled false
			/echo Main Assist set to me, ignoring burn queries now
		} else {		
			/dquery ${BurnAssistName} -q "BurnID" -o burnTempID
			/delay ${DanNetDelay} ${DanNet.Query.Received}
			/if (!${DanNet.Query.Received} && ${IsBurnEnabled}) {
				/echo Main Assist set to ${BurnAssistName} but they don't have dquery, failing for now
				/varset IsBurnQueryEnabled false
			} else {
				/varset IsBurnQueryEnabled true
				/echo Main Assist set to ${BurnAssistName} and dquery works
			}
		}
	}

	/if (${BurnTimer}) /return

	/if (${BurnID}) {
		/if (!${SpawnCount[id ${BurnID}]} || ${Spawn[id ${BurnID}].Type.Equal[Corpse]}) {
			/echo burn target dead
			/varset BurnID 0
			/varset burnTempID 0
			/if (${Me.Name.Equal[${Group.MainAssist.Name}]}) {
				/tribute personal off
				/tribute guild off
				/trophy personal off
				/trophy guild off
			}
		}
	}
	

	/varset BurnTimer 2s

	/if (${Group} && !${Me.Name.Equal[${BurnAssistName}]} && ${BurnAssistName.Length}) {
		/dquery ${BurnAssistName} -q "BurnID" -o burnTempID
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${Bool[${burnTempID}]} && ${BurnID} != ${burnTempID}) {
			/varset BurnID ${burnTempID}
			/echo ${BurnAssistName} told me to burn ${BurnID}
		}
	}
	

	/if (${IsBurnEnabled} && !${BurnID} && ${IsNamed[${Target.ID}]} && ${BurnID} != ${Target.ID} && ${Target.PctHPs} > 50) {
		/echo Named detected! Starting burn on ${Target.CleanName}
		/varset BurnID ${Target.ID}
		/varset burnTempID ${Target.ID}

		/if (${Me.Name.Equal[${BurnAssistName}]}) {
			/tribute personal on
			/tribute guild on
			/trophy personal on
			/trophy guild on
		}
	}

	/if (${Me.Class.ShortName.Equal[BER]} && ${IsPluginEnabled[mq2berzerker]} && ${Berzerker.BurnAlways} && ${BurnID} != ${Target.ID}) /ber burnalways off
	/if (${Me.Class.ShortName.Equal[BER]} && ${IsPluginEnabled[mq2berzerker]} && !${Berzerker.BurnAlways} && ${BurnID} > 0 && ${BurnID} == ${Target.ID}) /ber burnalways on
	
/return