package org.eclipse.kiso.cgw.ui

import java.nio.file.Files
import java.nio.file.Paths
import org.eclipse.core.resources.ResourcesPlugin
import org.eclipse.core.runtime.Path
import org.eclipse.swt.SWT
import org.eclipse.swt.events.ModifyEvent
import org.eclipse.swt.events.ModifyListener
import org.eclipse.swt.events.SelectionAdapter
import org.eclipse.swt.events.SelectionEvent
import org.eclipse.swt.layout.GridData
import org.eclipse.swt.layout.GridLayout
import org.eclipse.swt.widgets.Button
import org.eclipse.swt.widgets.Composite
import org.eclipse.swt.widgets.Label
import org.eclipse.swt.widgets.Text
import org.eclipse.ui.dialogs.ContainerSelectionDialog
import org.eclipse.ui.dialogs.WizardNewProjectCreationPage

class NewMitaProjectWizardPage extends WizardNewProjectCreationPage {
	Text kisoPath

	new() {
		super("NEWPRJ")
		setTitle("New Eclipse Mita Project")
		setDescription("Create a new Eclipse Mita project.")
	}

	override void createControl(Composite parent) {
		super.createControl(parent);		
		var Composite container = new Composite(control as Composite, SWT::NULL)
		var GridLayout layout = new GridLayout()
		container.setLayout(layout)
		layout.numColumns = 3
		layout.verticalSpacing = 9
		var Label label = new Label(container, SWT::NULL)
		label.setText("&Kiso root:")
		kisoPath = new Text(container, SWT::BORDER.bitwiseOr(SWT::SINGLE))
		var GridData gd = new GridData(GridData::FILL_HORIZONTAL)
		kisoPath.setLayoutData(gd)
		kisoPath.addModifyListener(([ModifyEvent e|dialogChanged()] as ModifyListener))
		var Button button = new Button(container, SWT::PUSH)
		button.setText("Browse...")
		button.addSelectionListener(new SelectionAdapter() {
			override void widgetSelected(SelectionEvent e) {
				handleBrowse()
			}
		})
		
		initialize()
		dialogChanged()
		setControl(container)
	}

	/** 
	 * Tests if the current workbench selection is a suitable container to use.
	 */
	def private void initialize() {
		kisoPath.setText("C:\\kiso")
	}

	/** 
	 * Uses the standard container selection dialog to choose the new value for
	 * the container field.
	 */
	def private void handleBrowse() {
		var ContainerSelectionDialog dialog = new ContainerSelectionDialog(getShell(),
			ResourcesPlugin::getWorkspace().getRoot(), false, "Select new file container")
		if (dialog.open() === ContainerSelectionDialog::OK) {
			var Object[] result = dialog.getResult()
			if (result.length === 1) {
				kisoPath.setText(((result.get(0) as Path)).toString())
			}
		}
	}

	/** 
	 * Ensures that both text fields are set.
	 */
	def private void dialogChanged() {
		val java.nio.file.Path kPath = Paths.get(getKisoPath());
		if(!Files.exists(kPath)) {
			updateStatus('''«getKisoPath()»' doesn't exist.''')
		}
		else if(!Files.isDirectory(kPath)) {
			updateStatus('''«getKisoPath()»' is not a directory.''')
		}
		else {
			updateStatus(null)
		}
	}

	def private void updateStatus(String message) {
		setErrorMessage(message)
		setPageComplete(message === null)
	}

	def String getKisoPath() {
		return kisoPath.getText()
	}
}
