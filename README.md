# cmpe344-project

This repository deploys `main.html` as a static GitHub Pages site.

## Deployment

GitHub Actions publishes the site automatically when changes are pushed to `main`.
The workflow copies `main.html` to `dist/index.html`, uploads that artifact, and
deploys it to the `github-pages` environment.

If Pages is not enabled yet, set the repository's Pages source to **GitHub
Actions** in GitHub's repository settings.
