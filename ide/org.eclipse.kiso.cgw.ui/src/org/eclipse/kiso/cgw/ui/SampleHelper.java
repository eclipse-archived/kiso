package org.eclipse.kiso.cgw.ui;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.FileSystemException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.function.Function;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.core.runtime.SubMonitor;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationListener;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.core.Launch;
import org.eclipse.debug.core.model.ISourceLocator;
import org.eclipse.debug.core.model.IStackFrame;
import org.eclipse.debug.internal.core.LaunchManager;


public class SampleHelper {
	public static void createProjectFromFiles(IProgressMonitor monitor, IProject destination, Map<String, Function<String, InputStream>> files, String sampleName, String kisoPath) throws CoreException {
		IProgressMonitor _monitor = SubMonitor.convert(monitor);
		_monitor.beginTask("Creating project from sample", 3);
		
		try {
			if (!destination.exists()) {
				destination.create(monitor);
			}
			
			destination.close(monitor);
			List<Path> copiedFiles = copyFilesIntoProject(_monitor, destination, files);
			_monitor.worked(1);
			
			if (!sampleName.equals(destination.getName())) {
				replaceOriginalProjectName(_monitor, copiedFiles, sampleName, destination.getName());
			}
			destination.open(monitor);
			_monitor.worked(1);
			
			destination.setDescription(ResourcesPlugin.getWorkspace().loadProjectDescription(destination.getLocation().append(".project")), monitor);
			
			setupCgwProject(monitor, destination, kisoPath);
			_monitor.worked(1);
		} finally {
			_monitor.done();
		}
	}
		
	private static List<Path> copyFilesIntoProject(IProgressMonitor rawMonitor, IProject destination, Map<String, Function<String, InputStream>> files) throws CoreException {
		IProgressMonitor monitor = SubMonitor.convert(rawMonitor);
		monitor.beginTask("Copying files into project", IProgressMonitor.UNKNOWN);
		
		final List<Path> copiedFiles = new LinkedList<>();
		try {
			IPath projectPath = destination.getLocation();
			for (Entry<String, Function<String, InputStream>> destAndSource : files.entrySet()) {
				File fileInProject = projectPath.append(destAndSource.getKey()).toFile();
				File containerInProject = fileInProject.getParentFile();
				if (!containerInProject.exists()) {
					containerInProject.mkdirs();
				}

				Path destinationPath = fileInProject.toPath();
				InputStream source = destAndSource.getValue().apply(destAndSource.getKey());
				try {
					Files.copy(source, destinationPath, StandardCopyOption.REPLACE_EXISTING);
					copiedFiles.add(destinationPath);
				} catch (IOException e) {
					throw new CoreException(
							new Status(IStatus.ERROR, Activator.PLUGIN_ID, "Error while copying project", e));
				} finally {
					source.close();
				}
			}
		} catch (IOException e) {
			throw new CoreException(new Status(IStatus.ERROR, Activator.PLUGIN_ID, "Error while copying project", e));
		} finally {
			monitor.done();
		}
		return copiedFiles;
	}

	private static void replaceOriginalProjectName(IProgressMonitor monitor, List<Path> filesToWorkOn, String originalName, String newName) throws CoreException {
		IProgressMonitor _monitor = SubMonitor.convert(monitor);
		_monitor.beginTask("Updating project name", filesToWorkOn.size());
		
		try {
			for(Path destinationPath : filesToWorkOn) {
				// replace name
				String content = new String(Files.readAllBytes(destinationPath));
				// first replace newName by originalName to ensure that e.g. MyLedsAndButtons
				// does not get replaced into MyMyLedsAndButton
				if (newName.contains(originalName)) {
					content = content.replaceAll(newName, originalName);
				}
				content = content.replaceAll(originalName, newName);
				
				Files.write(destinationPath, content.getBytes());
				
				// rename files
				String filename = destinationPath.getFileName().toString();
				int extensionPosition = filename.lastIndexOf('.');
				if(extensionPosition < 0) extensionPosition = filename.length();
				
				String filenameWithoutExtension = filename.substring(0, extensionPosition);
				String filenameExtension = filename.substring(extensionPosition);
				if(filenameWithoutExtension.equals(originalName)) {
					File newFilename = Paths.get(destinationPath.getParent().toString(), newName + filenameExtension).toFile();
					destinationPath.toFile().renameTo(newFilename);
				}
			}
		} catch (IOException e) {
			throw new CoreException(new Status(IStatus.ERROR, Activator.PLUGIN_ID, "Error while updating project name", e));
		} finally {
			_monitor.done();
		}
	}

	private static void setupCgwProject(IProgressMonitor monitor, IProject project, String kisoPath) throws CoreException {
		IProgressMonitor _monitor = SubMonitor.convert(monitor);
		_monitor.beginTask("Setting up CGW project", 3);
		
		try {
			project.refreshLocal(IResource.DEPTH_INFINITE, _monitor);
			
			project.open(monitor);
			final Path sourcePath = Paths.get(kisoPath);
			Path srcgen = Paths.get(project.getLocation().toPortableString()).resolve("src-gen");
			Files.createDirectory(srcgen);
			try {
				Files.createSymbolicLink(srcgen.resolve("kiso"), sourcePath);
			}
			catch(FileSystemException e) {
				// we might not have rights to make symlinks. let's try to copy.
				final Path kisoDir = srcgen.resolve("kiso");
				Files.walk(sourcePath).filter((Path p) -> {
					// don't copy .mita files: they would interfere with compilation
					return !p.toString().endsWith(".mita");
				}).forEach((Path p) -> {
					try {
						Path targetRelative = sourcePath.relativize(p);
						Files.copy(p, kisoDir.resolve(targetRelative));
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				});
			}
			_monitor.worked(1);
			
			String launchConfigPath = project.getLocation().append(project.getName() + ".launch").toOSString();
			LaunchManager launchManager = (LaunchManager) DebugPlugin.getDefault().getLaunchManager();
			ILaunchConfigurationListener lcl = new ILaunchConfigurationListener() {
				@Override
				public void launchConfigurationRemoved(ILaunchConfiguration configuration) {
				}
				@Override
				public void launchConfigurationChanged(ILaunchConfiguration configuration) {
					launchConfigurationAdded(configuration);
				}
				@Override
				public void launchConfigurationAdded(ILaunchConfiguration configuration) {
					launchManager.addLaunch(new Launch(configuration, ILaunchManager.DEBUG_MODE, null));
					launchManager.addLaunch(new Launch(configuration, ILaunchManager.RUN_MODE, null));
				}
			};
			launchManager.addLaunchConfigurationListener(lcl);
			launchManager.importConfigurations(new File[] {new File(launchConfigPath)}, monitor);
			launchManager.removeLaunchConfigurationListener(lcl);
			_monitor.worked(1);

			project.refreshLocal(IResource.DEPTH_INFINITE, _monitor);
			_monitor.worked(1);
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally {
			_monitor.done();
		}
	}
}
