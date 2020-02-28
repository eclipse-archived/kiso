---
title: "Workflow"
weight: 7
---

# How do I get my contribution into Kiso project?

## Recommended workflow
There are more than one way to achieve a goal in git world, however here we recommend the typical way of how contributors contribute to an open source project on github.

1. Create a fork  
Your contribution workflow starts with creating a fork of kiso project. Go to kiso github page https://github.com/eclipse/kiso and click on the fork button on the top right.

2. Setup your repo origin  
To differenciate your own origin and “kiso origin”, lets call “kiso origin” upstream.  
Let git be aware the fork you just created, naming origin to upstream:  
~~~~
git remote rename origin upstream  
~~~~  
then set your own fork to origin:
~~~~
git remote add origin https://github.com/<your github id>/kiso
~~~~
Now, your repos should be like this:
~~~~
git remote -v
origin   https://github.com/<your github id>/kiso (fetch)
origin   https://github.com/<your github id>/kiso (push)
upstream https://github.com/eclispe/kiso (fetch)
upstream https://github.com/eclipse/kiso (push)
~~~~

3. Create a topic branch on your own repo  
~~~~
git checkout master
git checkout -b feature/xxx
~~~~
In most cases, your branch is related to an issue, you should use:
~~~~
git checkout -b feature/#<issue_id>-xxx
~~~~


4. Make changes and commit  
This is the part where every git user should be familiar. If not, go to git or github user manual.  
However, please be aware of the commit message [checklist] (#commit-message-checklist) for kiso project.

5. Push your work to your own repo  
When you think it is time for your work to be reviewed, you should push your branch to create a PR:
~~~~
git push origin feature/#<issue_id>-xxx
~~~~

6. Create PR  
After issuing push command, you will see compare & pull request button, click on it and review the content yourself first and then click submit button.

7. Review  
Kiso project maintainers will review your work, and normally request for changes. There are a few rounds of review/rework, then your work would be potentially accepted and merged to kiso code base.  
**Note: sometimes, one review takes 1 – 2 weeks, therefore be patient. You can parallel work on other topics (in separate branch) while waiting.**

## Commit message format
In General, follow eclipse foundation's rule regarding the format of git commit message:
https://www.eclipse.org/projects/handbook/#resources-commit
