using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_RandomSoundPlayer : GISB_AudioPlayerTemplate<GISB_RandomSound>
    {
        private List<GISB_BaseAudioPlayer> instantiatedPlayers = new List<GISB_BaseAudioPlayer>();
        
        private List<int> indexes = new List<int>();
        private Queue<int> excludedIndexes = new Queue<int>();
        
        public GISB_RandomSoundPlayer(GISB_RandomSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance)
        {
            if (!RollForPlayProbability()) return;
            if(instantiatedPlayers.Count == 0)
            {
                for (int i = 0; i < audioObject.RandomPlaylist.Length; i++)
                {
                    indexes.Add(i);
                    instantiatedPlayers.Add(audioObject.RandomPlaylist[i].GetPlayer(this));
                }
            }
            
            if(instantiatedPlayers.Count > 0)
            {
                int index = GetRandomIndex();
                instantiatedPlayers[index].Play(activeParameters, gisbEventInstance);
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateParameters(activeParameters);
            }
        }

        private int GetRandomIndex()
        {
            int index = indexes[Random.Range(0, indexes.Count)];
            indexes.Remove(index);
            excludedIndexes.Enqueue(index);
            while(excludedIndexes.Count > audioObject.avoidLastPlayed)
            {
                indexes.Add(excludedIndexes.Dequeue());
            }
            return index;
        }
    }
}
