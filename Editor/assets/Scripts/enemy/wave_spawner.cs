using Game;
using System;
using System.Linq;
using Wiwa;

namespace WiwaApp
{
    using EntityId = System.UInt64;
    [Component]
    public struct WavesSpawner
    {
        public int maxEnemiesPerWave;
        public int maxWaveCount;
        public int currentWaveCount;
        public float timeBetweenWaves;
        public bool hasFinished;
        public bool hasTriggered;
        public int spawnOffset;
    }
    class WavesSpawnerSystem : Behaviour
    {
        ComponentIterator enemySpawnerIt;
        ComponentIterator currentWaveIt;
        EntityId currentWaveEntityId;
        float timer = 0.0f;
        bool previousWaveDestroy = false;
        bool debug = true;
        void Awake()
        {
            if (debug) Console.WriteLine("-- Starting Awake -- Enemy spawner");
            enemySpawnerIt.componentId = Constants.WI_INVALID_INDEX;
            enemySpawnerIt.componentIndex = Constants.WI_INVALID_INDEX;
            currentWaveIt.componentId = Constants.WI_INVALID_INDEX;
            currentWaveIt.componentIndex = Constants.WI_INVALID_INDEX;
            currentWaveEntityId = 0;
            previousWaveDestroy = false;
        }

        void Init()
        {
            if (debug) Console.WriteLine("-- Starting Init -- Enemy spawner");
            enemySpawnerIt = GetComponentIterator<WavesSpawner>();
            WavesSpawner enemySpawner = GetComponentByIterator<WavesSpawner>(enemySpawnerIt);
            timer = enemySpawner.timeBetweenWaves;
            enemySpawner.hasTriggered = false;
        }

        void Update()
        {
            if (debug) Console.WriteLine("-- Starting Update -- Enemy spawner");
            if (enemySpawnerIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref WavesSpawner enemySpawner = ref GetComponentByIterator<WavesSpawner>(enemySpawnerIt);
                //if (debug)
                //{
                //    Console.WriteLine("waveID");
                //    Console.WriteLine(currentWaveIt.componentId);
                //    Console.WriteLine(currentWaveIt.componentIndex);
                //    Console.WriteLine(currentWaveIt.componentSize);
                //}

                if (currentWaveIt.componentId != Constants.WI_INVALID_INDEX)
                {
                    if (debug) Console.WriteLine("Getting wave");
                    ref Wave currentWave = ref GetComponent<Wave>(currentWaveEntityId);
                    if (debug) Console.WriteLine("Checking wave");

                    if (debug) Console.WriteLine("No Wave");
                    if (!previousWaveDestroy)
                        previousWaveDestroy = CheckFinishWave(currentWave);

                    // Finish the spawner
                    if (enemySpawner.currentWaveCount >= enemySpawner.maxWaveCount && previousWaveDestroy)
                    {
                        enemySpawner.hasFinished = true;
                    }

                }

                // Timer before deploying next wave
                if (previousWaveDestroy && enemySpawner.hasFinished == false && enemySpawner.hasTriggered)
                {
                    timer -= Time.DeltaTime();
                    if (debug) Console.WriteLine("Timer -> " + timer);
                    if (timer < 0)
                    {
                        SpawnWave();
                        timer = enemySpawner.timeBetweenWaves;
                    }
                }
            }

            //if (debug) Console.WriteLine("-- Finish Update -- Enemy spawner");
        }



        private bool CheckFinishWave(Wave wave)
        {
            // Finish terminated waves
            if (wave.hasFinished)
            {
                if (debug) Console.WriteLine("Destroying wave " + currentWaveEntityId);
                currentWaveIt.componentId = Constants.WI_INVALID_INDEX;
                // Delete the wave entity entirely
                DestroyEntity(currentWaveEntityId);
                return true;
            }
            return false;
        }

        private void SpawnWave()
        {
            if (debug) Console.WriteLine("New wave and getting spawner comp");
            ref WavesSpawner enemySpawner = ref GetComponentByIterator<WavesSpawner>(enemySpawnerIt);

            enemySpawner.currentWaveCount += 1;

            // Create an empty entity
            string waveName = GetEntityName(m_EntityId);
            waveName += "_wave_" + enemySpawner.currentWaveCount;
            currentWaveEntityId = CreateEntityNamed(waveName);

            if (debug) Console.WriteLine("Getting transforms comp");
            ref Transform3D parent = ref GetComponent<Transform3D>(m_EntityId);
            ref Transform3D waveTransform = ref GetComponent<Transform3D>(currentWaveEntityId); ;
            waveTransform.LocalPosition = parent.worldMatrix.GetPosition();

            if (debug) Console.WriteLine("Adding wave comp");
            // Create a wave component and wave system
            ref Wave wave = ref AddComponent<Wave>(currentWaveEntityId);
            wave.currentEnemiesAlive = enemySpawner.maxEnemiesPerWave;
            wave.maxEnemies = enemySpawner.maxEnemiesPerWave;
            wave.hasFinished = false;
            if (debug) Console.WriteLine("Adding wave sys");
            ApplySystem<WaveSystem>(currentWaveEntityId);

            // Save the wave as current wave
            if (debug) Console.WriteLine("Getting wave comp it");
            currentWaveIt = GetComponentIterator<Wave>(currentWaveEntityId);
            if (debug)
            {
                Console.WriteLine("waveID");
                Console.WriteLine(currentWaveIt.componentId);
                Console.WriteLine(currentWaveIt.componentIndex);
                Console.WriteLine(currentWaveIt.componentSize);
            }
            ref Wave currentWave = ref GetComponentByIterator<Wave>(currentWaveIt);
            if (debug) Console.WriteLine("testing it");
            previousWaveDestroy = false;
        }
        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            if (id1 == m_EntityId && str2 == "PLAYER")
            {
                if (enemySpawnerIt.componentId != Constants.WI_INVALID_INDEX)
                {
                    ref WavesSpawner enemySpawner = ref GetComponentByIterator<WavesSpawner>(enemySpawnerIt);
                    if (!enemySpawner.hasTriggered)
                    {
                        SpawnWave();
                        enemySpawner.hasTriggered = true;
                    }

                }
            }
            
        }
    }
}