using Caliburn.Micro;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition.Hosting;
using System.ComponentModel.Composition;
using System.Linq;
using CHeroBot.Views;
using CHeroBot.Framework;

namespace CHeroBot
{
    public class AppBootstrapper : BootstrapperBase
    {
        private CompositionContainer m_Container;
   

        public AppBootstrapper()
        {
            this.Initialize();
        }

        protected override void Configure()
        {
            this.m_Container = new CompositionContainer(new AggregateCatalog(AssemblySource.Instance.Select(x => new AssemblyCatalog(x))));
            var batch = new CompositionBatch();

            batch.AddExportedValue<IWindowManager>(new WindowManager());
            batch.AddExportedValue<IEventAggregator>(new EventAggregator());
            batch.AddExportedValue(this.m_Container);

            this.m_Container.Compose(batch);
            Win32.LoadBotLibrary();
        }

        protected override object GetInstance(Type serviceType, string key)
        {
            var contract = string.IsNullOrEmpty(key) ? AttributedModelServices.GetContractName(serviceType) : key;
            var exports = this.m_Container.GetExportedValues<object>(contract).ToList();

            if (exports.Any())
                return exports.First();

            throw new Exception(string.Format("Could not locate any instances of contract {0}.", contract));
        }

        protected override IEnumerable<object> GetAllInstances(Type serviceType)
        {
            return this.m_Container.GetExportedValues<object>(AttributedModelServices.GetContractName(serviceType));
        }

        protected override void BuildUp(object instance)
        {
            this.m_Container.SatisfyImportsOnce(instance);
        }

        protected override void OnStartup(object sender, System.Windows.StartupEventArgs e)
        {
            this.DisplayRootViewFor<ShellViewModel>();
        }

        protected override void OnExit(object sender, EventArgs e)
        {
            base.OnExit(sender, e);
            Win32.FreeBotLibrary();
        }
    }
}
