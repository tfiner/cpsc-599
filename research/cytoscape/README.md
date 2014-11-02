# Cytoscape.js




## Documentation

You can find the documentation and downloads on the [project website](http://cytoscape.github.com/cytoscape.js).  This readme is mostly for developers of Cytoscape.js.

If you're looking to get an old version of the library, you can build off the associated tag or you can [browse the list of old builds](http://cytoscape.github.io/cytoscape.js/download/).  We very strongly recommend you use the latest version.  If you run into a bug on the latest version, report it on [the issue tracker](https://github.com/cytoscape/cytoscape.js/issues).  We'll try to fix the bug as soon as possible, and we'll give you a snapshot build that includes the fix for you to use until the next official bugfix release.




## Contributing to Cytoscape.js

Cytoscape.js is an open source project, and anyone interested is encouraged to contribute to Cytoscape.js.  We gladly accept pull requests.  If you are interested in regular contributions to Cytoscape.js, then we can arrange granting you permission to the repository by [contacting us](mailto:cytoscape-discuss@googlegroups.com?subject=Granting permission to Cytoscape.js repository).

If your pull request is a bugfix, please make changes to the master branch.  Otherwise, please make changes to the next version's branch (i.e. unstable).




## Acknowledgements

Arbor was used in one of Cytoscape.js' included layouts.  We made some modifications to the library, written by Samizdat Drafting Co., so that it would work with multiple instances of Cytoscape.js and that it would work on lesser browsers, like IE.  Information about this library can be found at the [Arbor website](http://arborjs.org/) and on [GitHub](https://github.com/maxkfranz/arbor) where the original code was forked.





## Adding source files

When adding source (.js) files to the repository, update the list of JS files in `gulpfile.js`.  You can update the references to these JS files in the tests and debug page et cetera with `gulp`:

 1. `gulp debugrefs` : Update the JS files referenced in the debug page (`debug/index.html`).
 1. `gulp testrefs` : Update the JS files referenced in the test page (`test/index.html`).
 1. `gulp testlist` : Update the JS test files referenced in the test page (`test/index.html`).

Or you can do them together via `gulp refs`.



## Build dependencies

Install `npm` and `gulp`.  Of course, `npm install` before using `gulp`.




## Build instructions

Run `gulp` in the console.  The main targets are:

**Building:**
 * `build` : build the library
 * `zip` : build the release ZIP
 * `clean` : clean the `build` directory
 * `dist` : update the distribution JS for npm, bower, etc.

**File references:**
 * `refs` : update all refs
  * `testrefs` : update JS lib file refs in the tests page
  * `testlist` : update list of test JS files in tests page
  * `debugrefs` : update JS lib file refs in debug page

**Testing:**
 * `test` : run the Mocha unit tests
 * `lint` : lint the JS sources via jshint
 * `watch` : update JS refs in HTML files (debug page, test page) automatically when JS files are added or deleted

**Documenation:**
 * `docs` : build the documentation template
 * `docsdemoshots` : get snapshots of demos referenced in the docs (requires PhantomJS, e.g. `brew update && brew install phantomjs`)
 * `docsmin` : build the documentation template with all resources minified
 * `docspub` : build the documentation for publishing (ZIPs, JS refs, etc.)





## Release instructions

 1. Update the `VERSION` environment variable, e.g. `export VERSION=1.2.3`.
 1. Confirm `VERSION` is picked up by gulp: `gulp version`.
 1. Confirm JS files pass linting: `gulp lint`
 1. Confirm all tests passing: `gulp test`
 1. Build and publish the release: `gulp pub`, and then...
  1. Make sure all your changes are pushed.
  1. Create a tag for this version in `git` (e.g. `gulp tag`)
  1. Copy the docs in `documentation` to the `gh-pages` branch and push (e.g. `gulp docspush`)
  1. Publish to npm (e.g. `gulp npm`)



## Tests

Mocha tests are found in the [test directory](https://github.com/cytoscape/cytoscape.js/tree/master/test).  The tests can be run in the browser or they can be run via Node.js (`gulp test`).




## Funding

Funding for Cytoscape.js and Cytoscape is provided by NRNB (U.S. National Institutes of Health, National Center for Research Resources grant numbers P41 RR031228 and GM103504) and by NIH grants 2R01GM070743 and 1U41HG006623. The following organizations help develop Cytoscape:

[ISB](http://www.systemsbiology.org) | 
[UCSD](http://www.ucsd.edu) | 
[MSKCC](http://cbio.mskcc.org) | 
[Pasteur](http://www.pasteur.fr) | 
[Agilent](http://www.agilent.com/) | 
[UCSF](http://www.ucsf.edu/) |
[Unilever](http://www.unilever.com) |
[Toronto](http://www.utoronto.ca) |
[NCIBI](http://portal.ncibi.org/gateway/index.html) |
[NRNB](http://nrnb.org)


