using Caliburn.Micro;
using CHeroBot.Framework;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Composition;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;

namespace CHeroBot.Views
{
    [Export]
    public class ShellViewModel : Conductor<IScreen>.Collection.OneActive, IHandle<IntPtr>
    {
        private readonly IEventAggregator m_MsgBus;
        private SearchViewModel m_SearchModel;
        private MainViewModel m_MainModel;
        private IntPtr m_BotWindow;


        [ImportingConstructor]
        public ShellViewModel(IEventAggregator msgBus)
        {
            this.m_MsgBus = msgBus;
            this.m_MsgBus.Subscribe(this);

            this.DisplayName = "CHeroBot : Clicker Heroes Automated Bot";
            this.m_SearchModel = new SearchViewModel(msgBus);
            this.m_MainModel = new MainViewModel();
        }

        protected override void OnViewLoaded(object view)
        {
            HwndSource source = PresentationSource.FromVisual((ShellView)view) as HwndSource;
            this.m_BotWindow = source.Handle;
            source.AddHook(new HwndSourceHook(this.WndProc));
        }

        protected override void OnInitialize()
        {
            this.ActivateItem(this.m_SearchModel);
        }

        public void Handle(IntPtr message)
        {
            this.m_MainModel.GameWindow = message;
            this.m_MainModel.BotWindow = this.m_BotWindow;
            this.ActivateItem(this.m_MainModel);
        }

        private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == 10025)
                this.ActivateItem(this.m_SearchModel);

            return IntPtr.Zero;
        }
    }
}
