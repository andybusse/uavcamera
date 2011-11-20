extern "C" void __cxa_pure_virtual()
{
#ifdef __AVR__
    asm("cli");
#endif
    while (1)
	;
}
