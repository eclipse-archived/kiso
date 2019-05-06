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
import org.eclipse.xtend.lib.annotations.Delegate
import java.nio.charset.Charset
import java.io.StringBufferInputStream
import java.io.BufferedReader
import java.io.InputStreamReader
import java.util.stream.Collectors
import java.io.StringReader
import java.io.BufferedOutputStream
import java.io.PipedOutputStream
import java.io.PipedInputStream
import java.util.regex.Pattern

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
		addFromBundle(bundle, files, "/template/debug.launch", project.name + ".launch", 
			#{
				"projectName" -> project.name,
				"workspacePath" -> project.workspace.root.location.toPortableString
			}
		)
		SampleHelper.createProjectFromFiles(rawMonitor, project, files, "MitaProjectTemplate", kisoPath)
	}

	def private static void addFromBundle(Bundle bundle, Map<String, Function<String, InputStream>> files, String pathInBundle, String pathInProject) {
		addFromBundle(bundle, files, pathInBundle, pathInProject, #{});
	}
	
	def private static void addFromBundle(Bundle bundle, Map<String, Function<String, InputStream>> files, String pathInBundle, String pathInProject, Map<String, String> variables) {
		files.put(pathInProject, [ String unused |
			try {
				val fileStream = FileLocator.openStream(bundle, Path.fromPortableString(pathInBundle), false)
				if(variables.empty) {
					return fileStream;
				}
				val content = new BufferedReader(new InputStreamReader(fileStream)).lines;
				val replaced = variables.entrySet.fold(content, [c, kv| c.map[line|
					line.replaceAll(Pattern.quote("${" + kv.key + "}"), kv.value);
				]]).map[it.bytes];
				
				val outputStream = new PipedOutputStream();
				val resultStream = new PipedInputStream(outputStream);
				
				new Thread([
					replaced.forEach[
						outputStream.write(it);
						outputStream.write("\n".bytes);
					]
					outputStream.close();
				]).start()				
				return resultStream;
			} catch (IOException e) {
				e.printStackTrace()
				return null
			}
		])
	}
}
