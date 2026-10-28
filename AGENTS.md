# Project conventions

## Repositories

- This is `iic-jku/LinAlgebra` (remote `jku`, formerly `martinjankoehler/LinAlgebra`), the
  maintained fork of `ediloren/LinAlgebra` (remote `upstream`, dormant). Remotes use SSH URLs
  (`git@github.com:…`).
- `iic-jku/FasterCap` builds it as the sibling `../LinAlgebra` (`add_subdirectory`); its CI
  checks out this fork's `master`, and its `ctest` runs the tests here too. Locally,
  `FasterCap_MJK` builds the `LinAlgebra` checkout, and `LinAlgebra_MJK` holds the branches.
- The repository stays in ediloren's fork network while PRs to upstream are open, so GitHub
  proposes `ediloren/LinAlgebra` as the base of a new PR: pick `iic-jku/LinAlgebra` `master`.
- `LinAlgebraTest` (`martinjankoehler/LinAlgebraTest`) compares timings and results of the
  original and the fork.

## Branches

- `master` is `upstream/master` plus merge commits (`--no-ff`) of feature branches only;
  `master-backup-<date>` branches keep earlier histories. Feature branches are never deleted.
- Changes offered upstream (`ediloren/LinAlgebra#1`–`#4`: `cmake-project-name-fix`,
  `correct-capitalized-includes`, `blas-optimization`, `unnecessary-debug-include`) live on
  branches based on `upstream/master`. Fix them with new commits on the branch, never by
  rewriting it (the PRs show the history), then merge the branch into `master` again.
- Fork-only changes (CI, AGENTS.md) go on branches based on `master`, e.g. `ci-and-agents`.
- Tests go on `regression-tests`, based on `master`: every fix gets a test there that fails
  without it. `master` merges `regression-tests` together with the fixes.

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

- There is no test suite here. Each bug fix gets a test that fails without it, comparing the
  BLAS path with the original loops on non-square, square, `1×N` and empty shapes.
- Check FasterCap after every change: build `FasterCap_MJK` against it and compare a
  reference run with the build before (same machine, same build type).
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
- Every PR for a bug references an issue: `Fixes #N` or `Part of #N`; without an issue,
  file one first. Issues in other repos are written `ediloren/LinAlgebra#N`,
  `iic-jku/FasterCap#N`.
- Issues show the smallest reproducing input and end with the versions used: commit, OS and
  CPU architecture, compiler, BLAS library and its integer model.
- Prefer bullet lists to long sentences chained with semicolons; no period at the end of a
  bullet. Mark identifiers as code with backticks, every mention.
- No AI attribution: no `Co-Authored-By` trailer, no "Generated with …" line.
- Before handing over, re-read your diff against `master` and check it against this file.
