extern "C" void fake();
__attribute__((constructor)) void _entry() {
    fake();
}