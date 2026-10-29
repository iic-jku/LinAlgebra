# Project conventions

## Repositories

- This is `iic-jku/LinAlgebra` (remote `jku`, formerly `martinjankoehler/LinAlgebra`), the
  maintained fork of `ediloren/LinAlgebra` (remote `upstream`, dormant). Remotes use SSH URLs
  (`git@github.com:…`).
- `iic-jku/FasterCap` builds it as the sibling `../LinAlgebra` (`add_subdirectory`); its CI
  checks out this fork's `master`, and its `ctest` runs the tests here too.
- Locally (`~/Source/FastFieldSolvers`), `LinAlgebra_MJK` is the working checkout;
  `LinAlgebra`, `LinAlgebra.orig` and `LinAlgebra_orig` are references only. FasterCap builds
  it through the symlink `work/LinAlgebra` (see FasterCap's AGENTS.md).
- The repository is standalone, not in ediloren's fork network: new PRs default to
  `iic-jku/LinAlgebra` `master`. The open upstream PRs `ediloren/LinAlgebra#1`–`#4` come
  from its branches.
- `LinAlgebraTest` (`martinjankoehler/LinAlgebraTest`) compares timings and results of the
  original and the fork.

## Branches

- Every change to code, CMake files or tests goes issue → branch → PR → `master`: one branch
  per issue, based on `master`, merged with a merge commit once its CI is green. The PR
  carries the fix together with its test.
- CI, `AGENTS.md`, `ReadMe.txt` and other docs, `.gitignore`: changes that don't touch the
  library are committed directly on `master`, without issue or PR.
- The branches of the open upstream PRs (`ediloren/LinAlgebra#1`–`#4`:
  `cmake-project-name-fix`, `correct-capitalized-includes`, `blas-optimization`,
  `unnecessary-debug-include`) stay based on `upstream/master`: fix them with new commits,
  never by rewriting them (the PRs show the history), then open a PR from the branch into
  `master`.
- A branch is deleted once its PR is merged, on GitHub and locally. The branches of open
  upstream PRs stay, and so do the `master-backup-<date>` branches with earlier histories.

## Scope

- One concern per branch; report unrelated findings instead of fixing them.
- Say in the commit body when behavior changes: results, signatures, build options.
- Fix root causes in the shared code, not with special cases.

## Code

- Keep each file's line endings (CRLF), indentation (tabs) and naming (`CLin_…`, `m_i…`).
  `STDAFX.H` stays the first include of every `.cpp`.
- `CLin_Matrix` stores rows contiguously (`m_pV`, row pointers `m_pRow`): BLAS calls use
  `CblasRowMajor`, and the leading dimension is the number of columns of the stored
  matrix. `CLin_subscript` is `unsigned long`; BLAS integers are `int` (OpenBLAS LP64) or
  `long` (Accelerate ILP64).
- BLAS functions abort or return garbage on invalid arguments: keep the size checks of the
  original loops (`CLin_assert`, `LINALG_BOUNDS_CHECK`), including empty vectors and
  matrices.
- CMake: BLAS is found here and exported with the `LinAlgebra` target (`PUBLIC` definitions,
  include directory and libraries). On macOS the Accelerate headers and library must come
  together (`BLA_VENDOR Apple`).
- Don't duplicate code: extend the shared code rather than adding a variant.

## Tests

- Run `ctest --test-dir ../work/build-LinAlgebra --output-on-failure` before calling work
  done (LinAlgebra builds on its own: `cmake -S . -B ../work/build-LinAlgebra`); FasterCap's
  `ctest` runs these tests too.
- Each bug fix gets a test that fails without it, in `tests/`: products compare the BLAS path
  with the plain loops on non-square, square, `1×N` and empty shapes.
- Check FasterCap after every change: build it from `../work/FasterCap` and run its `ctest`
  (see FasterCap's AGENTS.md).
- No loosened tolerances without a numerical explanation (summation order of BLAS kernels
  is one; differences beyond a few ulps per element are not).

## Git

- Martin reviews in detail and commits himself: leave changes uncommitted in the working
  copy. For a change that should become several commits, stage it and propose the split.
  Proposed commit messages are welcome as a starting point. Don't commit or push unless
  asked; never commit scratch files.
- Martin keeps WIP stashes: never chain `git stash push`/`pop`. Test staged changes via
  `git checkout-index -a --prefix=<dir>/` or a temporary worktree.
- Commit subject: `Area: verb phrase`, 6–12 words, no articles. Area is a file, class or
  function, e.g. `Mtx: rewrote matrix multiplication with faster BLAS-based impl`,
  `CMakeLists.txt: fixed project name`. Usually no body; one or two lines for a non-obvious
  why or a behavior change.
- Commit and PR texts contain only what the reader can't get from the issue, diff or CI.
  PR: issue reference plus 1–4 sentences, no Summary/Changes/Testing headings, and a
  proposed title in the style of the commit subjects.
- Every PR references its issue: `Fixes #N` or `Part of #N`; without an issue, file one
  first. Issues in other repos are written `ediloren/LinAlgebra#N`, `iic-jku/FasterCap#N`.
- Martin pushes and opens the PRs: for each finished branch, say whether it is pushed and
  propose the PR title and text.
- Issues show the smallest reproducing input and end with the versions used: commit, OS and
  CPU architecture, compiler, BLAS library and its integer model.
- Prefer bullet lists to long sentences chained with semicolons; no period at the end of a
  bullet. Mark identifiers as code with backticks, every mention.
- No AI attribution: no `Co-Authored-By` trailer, no "Generated with …" line.
- Before handing over, re-read your diff against `master` and check it against this file.
