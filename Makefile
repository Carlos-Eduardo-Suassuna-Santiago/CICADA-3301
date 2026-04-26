SHELL := /bin/bash

.PHONY: regression os-smoke os-build clean
.PHONY: vbox-setup vbox-validate vbox-test vbox-clean

regression:
	./scripts/regression_all.sh

os-smoke:
	$(MAKE) -C os boot-smoke

os-build:
	$(MAKE) -C os all

clean:
	$(MAKE) -C os clean

# VirtualBox Testing
vbox-setup:
	@./scripts/virtualbox_test_setup.sh CICADA-3301-TEST ./os/INITIUM.iso

vbox-validate:
	@./scripts/virtualbox_test_validate.sh CICADA-3301-TEST 60

vbox-test:
	@echo "Starting VirtualBox test workflow..."
	@$(MAKE) vbox-setup
	@echo ""
	@echo "Starting VM... (check docs/VIRTUALBOX_TESTING.md for details)"
	@VBoxManage startvm CICADA-3301-TEST --type headless
	@sleep 15
	@$(MAKE) vbox-validate

vbox-clean:
	@VBoxManage controlvm CICADA-3301-TEST poweroff 2>/dev/null || true
	@VBoxManage unregistervm CICADA-3301-TEST --delete 2>/dev/null || true
	@rm -f /tmp/cicada_virtualbox_*.log
	@echo "✓ VirtualBox cleanup done"
