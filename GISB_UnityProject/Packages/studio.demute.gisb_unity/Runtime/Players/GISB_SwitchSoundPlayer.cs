using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SwitchSoundPlayer : GISB_AudioPlayerTemplate<GISB_SwitchSound>
    {
        private Dictionary<string, GISB_BaseAudioPlayer> instantiatedPlayers = new Dictionary<string, GISB_BaseAudioPlayer>();

        public GISB_SwitchSoundPlayer(GISB_SwitchSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance)
        {
            if (!RollForPlayProbability()) return;
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
                        instantiatedPlayers[switchValue] = switchCase.audioObject.GetPlayer(this);
                        break;
                    }
                }
            }
            
            if(instantiatedPlayers.ContainsKey(switchValue))
            {
                instantiatedPlayers[switchValue].Play(activeParameters, gisbEventInstance);
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            foreach (KeyValuePair<string, GISB_BaseAudioPlayer> instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Value.UpdateParameters(activeParameters);
            }
        }
    }
}
