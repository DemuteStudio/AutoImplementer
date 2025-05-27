using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace GISB.Runtime
{
    public class GISB_BlendSoundPlayer : GISB_AudioPlayerTemplate<GISB_BlendSound>
    {
        private List<GISB_BaseAudioPlayer> instantiatedPlayers = new List<GISB_BaseAudioPlayer>();

        public GISB_BlendSoundPlayer(GISB_BlendSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(activeParameters, gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;
            if (instantiatedPlayers.Count != audioObject.BlendPlaylist.Length)
            {
                instantiatedPlayers.Clear();
                for (int i = 0; i < audioObject.BlendPlaylist.Length; i++)
                {
                    instantiatedPlayers.Add(audioObject.BlendPlaylist[i].GetPlayer(this));
                }
            }

            //fade in isn't passed on to children as it is evaluated on the highest container
            if (instantiatedPlayers.Count > 0)
            {
                foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
                {
                    instantiatedPlayer.Play(activeParameters, gisbEventInstance, 0.0f, scheduledTime);
                }
            }
        }

        public override void Stop()
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Stop();
            }
        }

        public override void UpdateAudioThread(double dspTime)
        {
            base.UpdateAudioThread(dspTime);
            
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateAudioThread(dspTime);
            }
        }

        public override void UpdateGameThread(float deltaTime)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateGameThread(deltaTime);
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateParameters(activeParameters);
            }
        }

        public override double GetDuration()
        {
            //Return duration of longest child
            double maxDuration = 0.0;
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                double duration = instantiatedPlayer.GetDuration();
                if (duration > maxDuration)
                {
                    maxDuration = duration;
                }
            }
            return maxDuration;
        }
    }
}

