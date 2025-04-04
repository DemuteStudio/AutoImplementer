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

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance)
        {
            if (!RollForPlayProbability()) return;
            if (instantiatedPlayers.Count != audioObject.BlendPlaylist.Length)
            {
                instantiatedPlayers.Clear();
                for (int i = 0; i < audioObject.BlendPlaylist.Length; i++)
                {
                    instantiatedPlayers.Add(audioObject.BlendPlaylist[i].GetPlayer(this));
                }
            }

            if (instantiatedPlayers.Count > 0)
            {
                foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
                {
                    instantiatedPlayer.Play(activeParameters, gisbEventInstance);
                }
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateParameters(activeParameters);
            }
        }
    }
}

