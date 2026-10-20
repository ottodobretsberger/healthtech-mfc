# MFCSDIAppln Migration Plan: MFC SDI -> WPF

## Purpose
This is a living conversion plan for migrating the current **MFC SDI multi-view desktop app** to an equivalent **WPF (.NET)** application.

Update this file after each completed step:
- Mark status changes
- Record implementation notes
- Capture deviations and follow-up tasks
- Keep parity checks current

---

## 1) Current Application Architecture (Assessment)

## 1.1 Solution and composition
- App type: Native C++ MFC SDI application
- Core pattern: MFC Document/View with one document and runtime-switchable views
- Entry point and startup: `CMFCSDIApplnApp` (`MFCSDIAppln.cpp`)
- Shell:
  - Main frame window: `CMainFrame` (`MainFrm.*`)
  - Toolbar + status bar
  - Toolbar combo box (`CToolComboBox`) drives view switching
- Document model: `CMFCSDIApplnDoc` (mostly placeholder serialization)
- Views:
  1. `CMFCSDIApplnView` (dashboard-like custom drawing)
  2. `CMyFrmView` (interactive protocol planning form)
  3. `CMyEditView` (editable structured report template)
- Resources:
  - Menus, toolbar, accelerators, about dialog in `MFCSDIAppln.rc`

## 1.2 Behavioral summary
- User can switch among three views using toolbar combo box.
- Main view displays a static dashboard UI using direct drawing.
- Form view computes a protocol plan from user inputs:
  - Exam type, weight, dose, scanner capacity
  - Derived values: contrast volume, slot duration, prep buffer
  - Extra keyboard/mouse shortcuts change model values
- Edit view provides a report template and auto-updates:
  - Completeness percentage
  - Status (`Draft` / `Ready for review`)
- Standard menu/toolbar commands exist for file, print, about, etc.

## 1.3 Architectural characteristics and migration implications
- Tight UI+logic coupling inside view classes -> should be separated into ViewModels/services in WPF.
- Runtime view switching is already conceptually aligned with WPF `ContentControl` + DataTemplates (or TabControl).
- Direct GDI drawing should be replaced by declarative XAML layouts and styling.
- Document serialization is mostly unimplemented, so persistence can be designed fresh in WPF.

---

## 2) MFC -> WPF Mapping (Functionality + UI)

| MFC Component / Pattern | Current Role | WPF Counterpart | Notes |
|---|---|---|---|
| `CMFCSDIApplnApp` (`CWinApp`) | App lifecycle/init | `App.xaml` + `App.xaml.cs` | Startup config, global resources, exception handling |
| `CMainFrame` (`CFrameWnd`) | Main shell window | `MainWindow.xaml` | Hosts menu/toolbar/status and active view region |
| `CToolBar` + combo box in toolbar | Commands + view selection | `ToolBar` + `ComboBox` bound to `SelectedView` | Prefer binding over event-reflection wiring |
| `CStatusBar` | App status indicators | `StatusBar` | Bind status text(s) via ShellViewModel |
| `CMFCSDIApplnDoc` (`CDocument`) | Document state/serialization placeholder | Domain models + optional persistence service | Could use JSON/XML persistence if needed |
| `SwitchView(CRuntimeClass*)` | Dynamic view replacement | View navigation via bound selected view model | `ContentControl` + DataTemplates recommended |
| `CMFCSDIApplnView` custom `OnDraw` | Dashboard rendering | `DashboardView.xaml` | Replace GDI drawing with Grid/Borders/TextBlocks/Cards |
| `CMyFrmView` controls + handlers | Input form + calculations | `ProtocolPlannerView.xaml` + `ProtocolPlannerViewModel` | Validation + commands + computed properties |
| `CMyEditView` + EN_CHANGE logic | Report editor and completeness updates | `ReportEditorView.xaml` + `ReportEditorViewModel` | Use text binding and automatic completeness recomputation |
| Message maps (`BEGIN_MESSAGE_MAP`) | Event routing | Routed events + `ICommand` + bindings | Avoid code-behind business logic |
| Resource IDs / `.rc` menus | Command surfaces | XAML Menu + Command bindings | Map file/edit/help actions |
| About dialog (`CDialogEx`) | App info | WPF `Window`/`ContentDialog` style about dialog | Modal `ShowDialog()` is sufficient |
| Accelerators | Keyboard shortcuts | `InputBindings` / `CommandBindings` | Add equivalent shortcuts as needed |

---

## 3) Detailed Feature Parity Matrix

| Feature | MFC Implementation | WPF Target | Parity Criteria |
|---|---|---|---|
| Multi-view switching | Toolbar combo changes active `CView` | Shell ComboBox switches active UserControl | Switching is immediate and state-safe |
| Dashboard view | `OnDraw` cards and text | Styled XAML dashboard cards | Visual sections and content remain equivalent |
| Protocol planner inputs | Combo + edits + calculate/reset buttons | Bound controls + `Calculate`/`Reset` commands | Same fields, limits, and defaults |
| Protocol calculations | In `RecalculatePlan()` | ViewModel/service method | Numeric output matches for same inputs |
| Planner keyboard/mouse behavior | Wheel and key shortcuts adjust values | Optional InputBindings/Preview handlers | Same or intentionally documented behavior |
| Report template | Pre-filled multiline text | Initial bound template text | Same section headings |
| Completeness + status | EN_CHANGE updates text | Reactive update from bound text | Percent/status update consistently |
| About dialog | `CAboutDlg` | About window/dialog | Accessible from Help menu |
| Print hooks | Standard MFC handlers | Optional WPF printing integration | Keep, defer, or explicitly de-scope |

---

## 4) Sequential Conversion Steps (Execution Plan)

Use these steps in order. Update status after each one.

| Step | Task | Deliverable | Status | Notes |
|---|---|---|---|---|
| 1 | Baseline the MFC behavior (screenshots + interaction notes for all 3 views) | Baseline evidence pack | ⬜ Not Started | Include keyboard/mouse shortcuts and edge cases |
| 2 | Create new WPF solution/project skeleton (.NET, x64) | Buildable WPF shell app | ⬜ Not Started | Keep naming close to original for traceability |
| 3 | Scaffold WPF artifacts (XAML views, ViewModels, command infrastructure) | Initial artifact scaffold committed | ⬜ Not Started | Explicitly create App/MainWindow/Views/ViewModels/RelayCommand |
| 4 | Implement shell layout (Menu, ToolBar, view selector ComboBox, StatusBar, content host) | MainWindow with shell structure | ⬜ Not Started | Bind shell controls to ShellViewModel |
| 5 | Introduce MVVM navigation state and view switching bindings | Working navigation infrastructure | ⬜ Not Started | Bind selected view from shell VM via ContentControl/DataTemplates |
| 6 | Migrate Main Dashboard view from GDI drawing to XAML cards/layout | `DashboardView` parity | ⬜ Not Started | Match semantic content, not pixel-perfect GDI |
| 7 | Migrate Protocol Planner form UI and input validation | `ProtocolPlannerView` parity | ⬜ Not Started | Apply same defaults and min/max clamping |
| 8 | Migrate planner computation logic into service/ViewModel | Verified calculation parity | ⬜ Not Started | Keep computation isolated and testable |
| 9 | Migrate Report Editor template + completeness/status logic | `ReportEditorView` parity | ⬜ Not Started | Reactive updates without recursion issues |
| 10 | Implement About dialog + command wiring + keyboard shortcuts | Functional menu/shortcuts parity | ⬜ Not Started | Keep behavior matrix updated |
| 11 | Draft and implement tests (parity tests + ViewModel unit tests) | Green test suite for migrated logic | ⬜ Not Started | Cover planner math, report completeness, and navigation behavior |
| 12 | Decide and implement persistence/print scope (equivalent or deferred with rationale) | Scope decision + implementation | ⬜ Not Started | Document explicit de-scopes |
| 13 | Perform parity QA pass against baseline and fix mismatches | Signed parity checklist | ⬜ Not Started | Include usability and validation behavior |
| 14 | Final cleanup, packaging, and handoff docs | Release-ready WPF app + docs | ⬜ Not Started | Include run/build instructions |

---

## 5) Suggested WPF Project Structure

```text
MFCSDIAppln.Wpf/
  App.xaml
  MainWindow.xaml
  Models/
  Services/
	ProtocolPlannerService.cs
  ViewModels/
	ShellViewModel.cs
	DashboardViewModel.cs
	ProtocolPlannerViewModel.cs
	ReportEditorViewModel.cs
  Views/
	DashboardView.xaml
	ProtocolPlannerView.xaml
	ReportEditorView.xaml
	AboutWindow.xaml
  Infrastructure/
	RelayCommand.cs
	Navigation/

MFCSDIAppln.Wpf.Tests/
  ViewModels/
	ProtocolPlannerViewModelTests.cs
	ReportEditorViewModelTests.cs
	ShellViewModelTests.cs
  Parity/
	ProtocolPlannerParityTests.cs
	ReportCompletenessParityTests.cs
```

---

## 6) Test Strategy (Required)

- Maintain two complementary test categories:
  1. **Parity tests**: assert WPF output/behavior equals MFC baseline for representative scenarios.
  2. **ViewModel unit tests**: validate logic in isolation (no UI thread dependency).
- Minimum required coverage:
  - Protocol planner calculation outputs for baseline inputs and boundary values.
  - Input clamping rules (weight, dose, scanner capacity).
  - Report completeness percentage and status transitions.
  - Shell view-switching state transitions.
- Add baseline test vectors captured from MFC into a test data file to avoid subjective parity checks.
- Gate migration progress: no feature step is considered complete without corresponding tests.

---

## 7) Non-Functional Requirements and Guardrails

- Preserve user-visible behavior before refactoring beyond parity.
- Keep business logic testable outside the UI layer.
- Prefer data binding and commands over code-behind event logic.
- Ensure keyboard accessibility and tab order.
- Keep all numeric limits and defaults aligned with MFC behavior unless intentionally changed.
- Record every intentional behavior change in this file.

---

## 8) Risks and Mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| Hidden behavior differences in message-map events | Functional mismatch | Capture baseline interactions before migration |
| Logic entangled with UI controls in MFC views | Regressions during port | Extract logic first into testable services |
| Keyboard/mouse shortcuts forgotten | UX regression | Maintain explicit shortcut checklist |
| Print/persistence scope ambiguity | Incomplete delivery | Make explicit keep/defer decision in step 10 |
| Visual mismatch from GDI -> XAML conversion | Stakeholder dissatisfaction | Review early screenshots and iterate styles |

---

## 9) Living Document Update Template

After each completed step, append an entry:

```markdown
### Update YYYY-MM-DD - Step <N>
- Owner:
- Summary:
- Files changed:
- Verification performed:
- Parity impact:
- Open issues / follow-ups:
- Next step:
```

Also update:
- Step table status
- Feature parity matrix
- Risks/mitigations (if new risks discovered)

---

## 10) Definition of Done

Migration is complete when:
- All three views are migrated and selectable from the WPF shell.
- Protocol planner calculations and limits match baseline behavior.
- Report editor completeness/status updates correctly.
- About/menu/shortcuts are wired and usable.
- Any deferred feature (print/persistence) is explicitly documented and approved.
- Parity tests and ViewModel unit tests are implemented and passing.
- Build/test instructions and migration notes are complete.

---

## Recommended Prompts (Optional)

1. "Analyze this MFC solution and generate a WPF MVVM shell with three switchable views matching existing view names."
2. "Port `CMyFrmView` to WPF: create ViewModel properties, validation limits, Calculate/Reset commands, and unit tests for output parity."
3. "Convert `CMFCSDIApplnView::OnDraw` dashboard into XAML cards preserving section content and hierarchy."
4. "Port `CMyEditView` report template logic to WPF with reactive completeness and status updates, avoiding recursive text update loops."
5. "Create a parity checklist comparing MFC and WPF behaviors for view switching, keyboard shortcuts, calculations, and report completion status."
6. "Given this migration plan, execute step-by-step and update the plan file after each completed step with evidence and remaining risks."