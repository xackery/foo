| Engage handles stick and attack logic
#include progg/core.inc

#event Progression "[MQ2] progression #1#"

Sub Event_Progression(string line, string expansion)
	/if (!${Defined[Progression]}) /declare Progression string outer
	/varset Progression ${expansion}
	/echo Doing progression for ${Progression}
/return


Sub ProgressionLogic
	/if (!${Defined[Progression]}) /declare Progression string outer
	/if (${Progression.Length} == 0 || ${Progression.Equal[NULL]}) /return	
	/call Progression "${Progression}"
	/if (!${Macro.Return}) {
		/varset Progression NULL
		/echo disabling progression, failed
	}
/return
