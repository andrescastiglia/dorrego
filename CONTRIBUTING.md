# Contributing to Dorrego

Thank you for considering contributing to **Dorrego**! We welcome contributions from everyone and appreciate your effort in improving this project. Please follow these guidelines to ensure a smooth collaboration.

---

## How Can You Contribute?

### 🐛 Reporting Bugs
1. Check the [issue tracker](https://github.com/andrescastiglia/dorrego/issues) to ensure the bug hasn't been reported already.
2. Create a new issue and include:
   - A clear and concise description of the problem.
   - Steps to reproduce the bug.
   - Expected and actual results.
   - Any relevant screenshots, logs, or error messages.
3. Tag the issue with `bug`.

### 💡 Suggesting Features
1. Search the [issue tracker](https://github.com/andrescastiglia/dorrego/issues) to ensure your suggestion hasn't been discussed.
2. Open a new issue tagged with `enhancement` and include:
   - A detailed description of the feature or improvement.
   - Why it would benefit the project.

### 🔧 Fixing Issues or Adding Features
1. Look for issues tagged `good first issue` or `help wanted` to get started.
2. **Always work in a branch off `develop`:**
   - Create a new branch based on `develop`:
     ```bash
     git checkout develop
     git pull origin develop
     git checkout -b feature/your-feature-name
     ```

   - **Branch names**:
        - Feature Branches: These branches are used for developing new features. Use the prefix `feature/`. For instance, `feature/login-system`.
        - Bugfix Branches: These branches are used to fix bugs in the code. Use the prefix `bugfix/`. For example, `bugfix/header-styling`.
        - Hotfix Branches: These branches are made directly from the production branch to fix critical bugs in the production environment. Use the prefix `hotfix/`. For instance,`hotfix/critical-security-issue`.
        - Release Branches: These branches are used to prepare for a new production release. They allow for last-minute dotting of i’s and crossing t’s. Use the prefix `release/`. For example, `release/v1.0.1`.

   - **Examples**
     ```bash
     git checkout -b feature/your-feature-name
     ```

3. Make your changes:
   - Ensure your code adheres to Rust’s best practices and project-specific guidelines.
   - Write or update tests for your changes.
   - Format your code using `rustfmt`:
     ```bash
     cargo fmt
     ```
   - Check for lints using `clippy`:
     ```bash
     cargo clippy --all-targets --all-features -- -D warnings
     ```
   - Run tests to verify everything works:
     ```bash
     trunk build
     ```
4. **Write clear commit messages following this convention**:
   - Use the format:
     ```
     <type>(<scope>): <description>
     ```
   - **Types**:
     - `feat`: Adding a new feature.
     - `fix`: Fixing a bug.
     - `docs`: Changes to documentation.
     - `style`: Code style or formatting changes (non-functional).
     - `refactor`: Code changes that neither fix a bug nor add a feature.
     - `test`: Adding or updating tests.
     - `chore`: Miscellaneous tasks (e.g., updating dependencies).
   - **Example**:
     ```bash
     git commit -m "feat(auth): add validation"
     ```
5. Push your branch:
   ```bash
   git push origin feature/your-feature-name
   ```