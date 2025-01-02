using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SwitchSoundPlayer : GISB_AudioObjectPlayer
    {
        private GISB_SwitchSound audioObject;

        private Dictionary<string, GISB_AudioObjectPlayer> instantiatedPlayers = new Dictionary<string, GISB_AudioObjectPlayer>();
        
        public GISB_SwitchSoundPlayer(GISB_SwitchSound audioObject)
        {
            this.audioObject = audioObject;
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance)
        {
            string switchValue = audioObject.DefaultValue;
            if(activeParameters.TryGetValue(audioObject.ParameterName, out string parameter))
            {
                switchValue = parameter;
            }

            if (!instantiatedPlayers.ContainsKey(switchValue))
            {
                //Create new player for this switch value
                foreach (GISB_SwitchSound.SwitchCase switchCase in audioObject.SwitchCases)
                {
                    if (switchCase.parameterValue == switchValue)
                    {
                        if(switchCase.audioObject == null)
                        {
                            break;
                        }
                        instantiatedPlayers[switchValue] = switchCase.audioObject.GetPlayer();
                        break;
                    }
                }
            }
            
            if(instantiatedPlayers.ContainsKey(switchValue))
            {
                instantiatedPlayers[switchValue].Play(activeParameters, gisbEventInstance);
            }
        }
    }
}
