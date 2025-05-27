using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SwitchSoundPlayer : GISB_AudioPlayerTemplate<GISB_SwitchSound>
    {
        private Dictionary<string, GISB_BaseAudioPlayer> instantiatedPlayers = new Dictionary<string, GISB_BaseAudioPlayer>();
        
        private string currentSwitchValue = string.Empty;

        public GISB_SwitchSoundPlayer(GISB_SwitchSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(activeParameters, gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;
            currentSwitchValue = audioObject.DefaultValue;
            if(activeParameters.TryGetValue(audioObject.ParameterName, out string parameter))
            {
                currentSwitchValue = parameter;
            }

            if (!instantiatedPlayers.ContainsKey(currentSwitchValue))
            {
                //Create new player for this switch value
                foreach (GISB_SwitchSound.SwitchCase switchCase in audioObject.SwitchCases)
                {
                    if (switchCase.parameterValue == currentSwitchValue)
                    {
                        if(switchCase.audioObject == null)
                        {
                            break;
                        }
                        instantiatedPlayers[currentSwitchValue] = switchCase.audioObject.GetPlayer(this);
                        break;
                    }
                }
            }
            
            if(instantiatedPlayers.ContainsKey(currentSwitchValue))
            {
                instantiatedPlayers[currentSwitchValue].Play(activeParameters, gisbEventInstance, 0.0f, scheduledTime);
            }
        }

        public override void Stop()
        {
            foreach (KeyValuePair<string, GISB_BaseAudioPlayer> instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Value.Stop();
            }
        }

        public override void UpdateAudioThread(double dspTime)
        {
            base.UpdateAudioThread(dspTime);

            foreach (KeyValuePair<string, GISB_BaseAudioPlayer> instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Value.UpdateAudioThread(dspTime);
            }
        }

        public override void UpdateGameThread(float deltaTime)
        {
            foreach (KeyValuePair<string, GISB_BaseAudioPlayer> instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Value.UpdateGameThread(deltaTime);
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            foreach (KeyValuePair<string, GISB_BaseAudioPlayer> instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Value.UpdateParameters(activeParameters);
            }
        }

        public override double GetDuration()
        {
            if (currentSwitchValue == string.Empty) return 0.0f;
            return instantiatedPlayers[currentSwitchValue].GetDuration();
        }
    }
}
