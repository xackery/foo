| Count will count how many of an item in advloot or on cursor is on crew

#event Count "[MQ2] count"

Sub Event_Count
	/call CountCursor
	/call CountAdvLoot
/return


Sub CountCursor
	/if (!${Cursor.ID}) /return
	/call CrewFind "${Cursor.Name}"
/return

sub CountAdvLoot
	/declare itemIndex int local
	/for itemIndex 1 to ${AdvLoot.SCount}		
		/call CrewFind "${AdvLoot.SList[${itemIndex}]}"
	/next itemIndex
/return

Sub CrewFind(itemName)
	/declare cn string local
	/declare crewIndex int local	
	/declare dout string local

	/declare total int local
	/for crewIndex 1 to ${Math.Calc[${DanNet.Peers.Count[|]}]}
		/varset total 0
		/varset cn ${DanNet.Peers.Token[${crewIndex},|]}
		/dquery ${cn} -q "FindItemCount[=${itemName}]" -o dout
		/delay 1s ${DanNet.Query.Received}
		/if (${cn.Equal[${Me.Name}]}) /varset dout ${FindItemCount[=${itemName}]}
		/if (${Int[${dout}]}) /varset total ${dout}
		
		/dquery ${cn} -q "FindItemCount[=${itemName}]" -o dout
		/delay 1s ${DanNet.Query.Received}
		/if (${cn.Equal[${Me.Name}]}) /varset dout ${FindItemBankCount[=${itemName}]}
		/if (!${Int[${dout}]}) /continue
		
		/if (${total}>0) {
			/varcalc total ${total}+${dout}
		}
		/echo ${cn} has ${total}x ${itemName}
		/return ${total}
	/next crewIndex	
/return 0