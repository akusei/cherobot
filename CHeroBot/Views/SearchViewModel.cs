using Caliburn.Micro;
using CHeroBot.Framework;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace CHeroBot.Views
{
    [Export]
    public class SearchViewModel : Screen
    {
        private const string CLASS_NAME = "ApolloRuntimeContentWindow";
        private const string WINDOW_NAME = "Clicker Heroes";

        private readonly IEventAggregator m_MsgBus;
        private string m_Status;
        private IntPtr m_GameWindow;
        private Task m_FindTask;
        private Task m_CloseTask;
        private bool m_Quit;


        public SearchViewModel(IEventAggregator msgBus)
        {
            this.m_MsgBus = msgBus;
        }

        protected override void OnActivate()
        {
            this.m_GameWindow = Win32.FindWindow(SearchViewModel.CLASS_NAME, SearchViewModel.WINDOW_NAME);
            if (this.m_GameWindow == IntPtr.Zero)
            {
                this.Status = "Waiting for game to start, click Cancel to quit...";
                this.m_FindTask = Task.Factory.StartNew(this.FindThread);
            }
            else
            {
                this.Status = "CHeroBot must be running before the game, restart the game to try again or click Cancel to quit...";
                this.m_CloseTask = Task.Factory.StartNew(this.CloseThread);
            }
        }

        protected override void OnDeactivate(bool close)
        {
            this.m_Quit = true;
            List<Task> tasks = new List<Task>();
            if (this.m_FindTask != null)
                tasks.Add(this.m_FindTask);
            if (this.m_CloseTask != null)
                tasks.Add(this.m_CloseTask);

            Task.WaitAll(tasks.ToArray());
        }

        private void CloseThread()
        {
            while (this.m_GameWindow != IntPtr.Zero && !this.m_Quit)
            {
                this.m_GameWindow = Win32.FindWindow(SearchViewModel.CLASS_NAME, SearchViewModel.WINDOW_NAME);
                if (this.m_GameWindow == IntPtr.Zero)
                {
                    this.Status = "Waiting for game to start, click Cancel to quit...";
                    this.m_FindTask = Task.Factory.StartNew(this.FindThread);
                }
                else
                {
                    if (!this.m_Quit)
                        Thread.Sleep(100);
                }
            }
        }

        private void FindThread()
        {
            while (this.m_GameWindow == IntPtr.Zero && !this.m_Quit)
            {
                this.m_GameWindow = Win32.FindWindow(SearchViewModel.CLASS_NAME, SearchViewModel.WINDOW_NAME);
                if (this.m_GameWindow != IntPtr.Zero)
                {
                    this.m_MsgBus.PublishOnUIThreadAsync(this.m_GameWindow);
                    return;
                }
                else
                {
                    if (!this.m_Quit)
                        Thread.Sleep(100);
                }
            }
        }

        public string Status
        {
            get { return this.m_Status; }
            private set
            {
                if (string.CompareOrdinal(this.m_Status, value) != 0)
                {
                    this.m_Status = value;
                    this.NotifyOfPropertyChange();
                }
            }
        }

        public void CmdClick()
        {
            this.m_Quit = true;
            List<Task> tasks = new List<Task>();
            if (this.m_FindTask != null)
                tasks.Add(this.m_FindTask);
            if (this.m_CloseTask != null)
                tasks.Add(this.m_CloseTask);

            Task.WaitAll(tasks.ToArray());

            Application.Current.Shutdown();
        }
    }
}
