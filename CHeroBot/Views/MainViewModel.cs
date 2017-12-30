using Caliburn.Micro;
using CHeroBot.Framework;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace CHeroBot.Views
{
    [Export]
    public class MainViewModel : Screen
    {
        private bool m_LevelsEnabled;
        private bool m_UpgradesEnabled;
        private bool m_IsPaused;
        private uint m_UpgradeDelay;
        private uint m_LevelDelay;
        private uint m_OldBossDelay;
        private uint m_RollbackLevels;

        public MainViewModel()
        {
        }

        protected override void OnActivate()
        {
            this.m_LevelsEnabled = true;
            this.m_UpgradesEnabled = true;
            this.m_UpgradeDelay = 30;
            this.m_LevelDelay = 70;
            this.m_OldBossDelay = 120;
            this.m_RollbackLevels = 8;
            this.m_IsPaused = false;

            Win32.StartBot(this.BotWindow, this.GameWindow);
        }

        protected override void OnDeactivate(bool close)
        {
            Win32.StopBot();
        }

        public void CmdPause()
        {
            this.IsPaused = Win32.TogglePause();
        }

        public IntPtr GameWindow { get; set; }

        public IntPtr BotWindow { get; set; }

        public bool LevelsEnabled
        {
            get { return this.m_LevelsEnabled; }
            set
            {
                this.m_LevelsEnabled = value;
                bool ret = Win32.ToggleLevels();
                if (ret != !this.m_LevelsEnabled)
                {
                    this.m_LevelsEnabled = !ret;
                    this.NotifyOfPropertyChange();
                }
            }
        }

        public bool UpgradesEnabled
        {
            get { return this.m_UpgradesEnabled; }
            set
            {
                this.m_UpgradesEnabled = value;
                bool ret = Win32.ToggleUpgrades();
                if (ret != !this.m_UpgradesEnabled)
                {
                    this.m_UpgradesEnabled = !ret;
                    this.NotifyOfPropertyChange();
                }
            }
        }

        public bool IsPaused
        {
            get { return this.m_IsPaused; }
            set
            {
                if (this.m_IsPaused != value)
                {
                    this.m_IsPaused = value;
                    this.NotifyOfPropertyChange();
                }
            }
        }

        public uint UpgradeDelay
        {
            get { return this.m_UpgradeDelay; }
            set
            {
                if (this.m_UpgradeDelay != value)
                {
                    this.m_UpgradeDelay = value;
                    Win32.SetUpgradeInterval(this.m_UpgradeDelay * 1000);
                }
            }
        }

        public uint LevelDelay
        {
            get { return this.m_LevelDelay; }
            set
            {
                if (this.m_LevelDelay != value)
                {
                    this.m_LevelDelay = value;
                    Win32.SetLevelDelay(this.m_LevelDelay * 1000);
                }
            }
        }

        public uint OldBossDelay
        {
            get { return this.m_OldBossDelay; }
            set
            {
                if (this.m_OldBossDelay != value)
                {
                    this.m_OldBossDelay = value;
                    Win32.SetOldBossDelay(this.m_OldBossDelay * 1000);
                }
            }
        }

        public uint RollbackLevels
        {
            get { return this.m_RollbackLevels; }
            set
            {
                if (this.m_RollbackLevels != value)
                {
                    this.m_RollbackLevels = value;
                    Win32.SetRollbackCount(this.m_RollbackLevels);
                }
            }
        }
    }
}
