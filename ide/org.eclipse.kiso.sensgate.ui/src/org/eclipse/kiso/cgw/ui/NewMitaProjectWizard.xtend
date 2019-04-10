package org.eclipse.kiso.cgw.ui

import java.lang.reflect.InvocationTargetException
import org.eclipse.core.resources.IProject
import org.eclipse.core.runtime.CoreException
import org.eclipse.core.runtime.IProgressMonitor
import org.eclipse.core.runtime.SubMonitor
import org.eclipse.jface.dialogs.MessageDialog
import org.eclipse.jface.operation.IRunnableWithProgress
import org.eclipse.jface.viewers.IStructuredSelection
import org.eclipse.jface.wizard.Wizard
import org.eclipse.ui.INewWizard
import org.eclipse.ui.IWorkbench
import org.eclipse.ui.IWorkbenchPage
import org.eclipse.ui.IWorkbenchWizard
import org.eclipse.ui.PartInitException
import org.eclipse.ui.PlatformUI
import org.eclipse.ui.ide.IDE

class NewMitaProjectWizard extends Wizard implements INewWizard {
	protected var NewMitaProjectWizardPage page

	new() {
		super()
		setNeedsProgressMonitor(true)
	}

	/** 
	 * Adding the page to the wizard.
	 */
	override void addPages() {
		page = new NewMitaProjectWizardPage()
		addPage(page)
	}

	/** 
	 * This method is called when 'Finish' button is pressed in
	 * the wizard. We will create an operation and run it
	 * using wizard as execution context.
	 */
	override boolean performFinish() {
		val IProject projectHandle = page.getProjectHandle()
		val kPath = page.kisoPath
		var IRunnableWithProgress op = [ IProgressMonitor monitor |
			try {
				doFinish(projectHandle, monitor, kPath)
			} catch (CoreException e) {
				throw new InvocationTargetException(e)
			} finally {
				monitor.done()
			}
		]
		try {
			getContainer().run(true, false, op)
			var IWorkbenchPage activeWorkbenchPage = PlatformUI.getWorkbench().getActiveWorkbenchWindow().
				getActivePage()
			IDE.openEditor(activeWorkbenchPage, projectHandle.getFile("application.mita"), true)
		} catch (InterruptedException e) {
			return false
		} catch (InvocationTargetException e) {
			var Throwable realException = e.getTargetException()
			MessageDialog.openError(getShell(), "Error", realException.getMessage())
			return false
		} catch (PartInitException e) {
			e.printStackTrace()
		}

		return true
	}

	/** 
	 * The worker method. It will create the project.
	 */
	def protected void doFinish(IProject project, IProgressMonitor rawMonitor, String kisoPath) throws CoreException {
		var IProgressMonitor monitor = SubMonitor.convert(rawMonitor)
		monitor.beginTask('''Creating «project.getName()»''', IProgressMonitor.UNKNOWN)
		try {
			EclipseMitaProjectGenerator.create(project, monitor, kisoPath)
		} finally {
			monitor.done()
		}
	}

	/** 
	 * We will accept the selection in the workbench to see if
	 * we can initialize from it.
	 * @see IWorkbenchWizard#init(IWorkbench, IStructuredSelection)
	 */
	override void init(IWorkbench workbench, IStructuredSelection selection) {
	}
}
