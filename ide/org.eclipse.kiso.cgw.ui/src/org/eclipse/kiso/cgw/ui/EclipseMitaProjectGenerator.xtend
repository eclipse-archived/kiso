package org.eclipse.kiso.cgw.ui

import java.io.IOException
import java.io.InputStream
import java.util.HashMap
import java.util.Map
import java.util.function.Function
import org.eclipse.core.resources.IProject
import org.eclipse.core.runtime.CoreException
import org.eclipse.core.runtime.FileLocator
import org.eclipse.core.runtime.IProgressMonitor
import org.eclipse.core.runtime.Path
import org.eclipse.core.runtime.Platform
import org.osgi.framework.Bundle

class EclipseMitaProjectGenerator {
	/** 
	 * Creates a new Eclipse Mita project
	 * @param project the project to create
	 * @param rawMonitor the progress monitor
	 * @param kisoPath the path to the kiso git repository
	 * @throws CoreException
	 */
	def static void create(IProject project, IProgressMonitor rawMonitor, String kisoPath) throws CoreException {
		var Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID)
		var Map<String, Function<String, InputStream>> files = new HashMap()
		addFromBundle(bundle, files, "/template/application.mita.template", "application.mita")
		addFromBundle(bundle, files, "/template/project", ".project")
		addFromBundle(bundle, files, "/template/cproject", ".cproject")
		SampleHelper.createProjectFromFiles(rawMonitor, project, files, "MitaProjectTemplate", kisoPath)
	}

	def private static void addFromBundle(Bundle bundle, Map<String, Function<String, InputStream>> files,
		String pathInBundle, String pathInProject) {
		files.put(pathInProject, [ String unused |
			try {
				return FileLocator.openStream(bundle, Path.fromPortableString(pathInBundle), false)
			} catch (IOException e) {
				e.printStackTrace()
				return null
			}
		])
	}
}
