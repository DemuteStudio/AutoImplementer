using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Random = UnityEngine.Random;

namespace GISB.Runtime
{
    public class GISB_RandomSoundPlayer : GISB_AudioPlayerTemplate<GISB_RandomSound>
    {
        public class RandomizationValues
        {
            private List<int> indexes;
            private Queue<int> excludedIndexes;
            private int avoidLastPlayed;
            
            public RandomizationValues(int count, int avoidLastPlayed)
            {
                
                indexes = new List<int>(count);
                for (int i = 0; i < count; i++)
                {
                    indexes.Add(i);
                }
                excludedIndexes = new Queue<int>();
                this.avoidLastPlayed = Mathf.Min(avoidLastPlayed, count - 1); // Ensure we don't avoid more than available sounds
            }

            public int GetRandomIndex()
            {
                int index = indexes[Random.Range(0, indexes.Count)];
                indexes.Remove(index);
                excludedIndexes.Enqueue(index);
                while(excludedIndexes.Count > avoidLastPlayed)
                {
                    indexes.Add(excludedIndexes.Dequeue());
                }
                return index;
            }
        }
        
        private int currentIndex = -1;
        
        private List<GISB_BaseAudioPlayer> instantiatedPlayers = new List<GISB_BaseAudioPlayer>();
        
        
        public GISB_RandomSoundPlayer(GISB_RandomSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;

            if (!gisbEventInstance.ownerComponent.randomizationValuesCache.ContainsKey(audioObject))
            {
                gisbEventInstance.ownerComponent.randomizationValuesCache[audioObject] =
                    new RandomizationValues(audioObject.RandomPlaylist.Length, audioObject.avoidLastPlayed);
            }
            
            if(instantiatedPlayers.Count == 0)
            {
                for (int i = 0; i < audioObject.RandomPlaylist.Length; i++)
                {
                    instantiatedPlayers.Add(audioObject.RandomPlaylist[i].GetPlayer(this));
                }
            }
            
            //fade in isn't passed on to children as it is evaluated on the highest container
            if(instantiatedPlayers.Count > 0)
            {
                currentIndex = GetRandomIndex();
                instantiatedPlayers[currentIndex].Play(gisbEventInstance, 0.0f, scheduledTime);
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
        
        public override void UpdateParameters(Dictionary<string, float> activeParameters)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateParameters(activeParameters);
            }
        }

        public override double GetDuration()
        {
            //Return duration of currently playing sound
            if (currentIndex < 0) return 0.0f; // No sound is currently playing
            return instantiatedPlayers[currentIndex].GetDuration();
        }

        private int GetRandomIndex()
        {
            return ownerInstance.ownerComponent.randomizationValuesCache[audioObject].GetRandomIndex();
        }

        public override bool IsPlaying()
        {
            if (currentIndex < 0) return false; // No sound is currently playing
            return instantiatedPlayers[currentIndex].IsPlaying();
        }
    }
}
