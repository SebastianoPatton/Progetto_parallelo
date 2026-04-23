# ============================================================
#  Makefile – Mandelbrot Set (Ottimizzato per Windows)
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -I./include
OMPFLAGS := -fopenmp

# Target
SERIAL_SRC := src/mandelbrot_serial.cpp
SERIAL_BIN := mandelbrot_serial.exe

PARALLEL_SRC := src/mandelbrot_omp.cpp
PARALLEL_BIN := mandelbrot_omp.exe

# ============================================================
#  Targets principali
# ============================================================

.PHONY: all serial parallel run run_parallel test clean help

all: serial parallel

serial: $(SERIAL_BIN)

parallel: $(PARALLEL_BIN)

$(SERIAL_BIN): $(SERIAL_SRC) include/mandelbrot.h include/stb_image_write.h
	$(CXX) $(CXXFLAGS) -o $@ $<
	@echo ">>> Compilazione seriale completata: $(SERIAL_BIN)"

$(PARALLEL_BIN): $(PARALLEL_SRC) include/mandelbrot.h include/stb_image_write.h
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -o $@ $<
	@echo ">>> Compilazione parallela completata: $(PARALLEL_BIN)"

# ============================================================
#  Esecuzione
# ============================================================

run: serial
	-@mkdir outputs 2>nul
	@echo === Esecuzione Seriale === > outputs/tempi_esecuzione.txt
	.\$(SERIAL_BIN) inputs/params.txt outputs/mandelbrot.png >> outputs/tempi_esecuzione.txt
	@echo ">>> Esecuzione completata! Controlla il file outputs/tempi_esecuzione.txt"

run_parallel: parallel
	-@mkdir outputs 2>nul
	@echo === Esecuzione Parallela === > outputs/tempi_esecuzione.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/params.txt outputs/mandelbrot_parallel.png >> outputs/tempi_esecuzione.txt
	@echo ">>> Esecuzione parallela completata! Controlla il file outputs/tempi_esecuzione.txt"

# ============================================================
#  Test – confronta risoluzioni diverse (studia la scalabilità)
# ============================================================

test: serial
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test.txt
	@echo TEST SCALABILITA' SERIALE >> outputs/tempi_test.txt
	@echo ======================================= >> outputs/tempi_test.txt
	
	@echo Esecuzione Test 512x512 in corso...
	@echo --- Test 512x512 --- >> outputs/tempi_test.txt
	.\$(SERIAL_BIN) inputs/p512.txt outputs/test_512.png >> outputs/tempi_test.txt
	
	@echo Esecuzione Test 1024x1024 in corso...
	@echo --- Test 1024x1024 --- >> outputs/tempi_test.txt
	.\$(SERIAL_BIN) inputs/p1024.txt outputs/test_1024.png >> outputs/tempi_test.txt
	
	@echo Esecuzione Test 2048x2048 in corso...
	@echo --- Test 2048x2048 --- >> outputs/tempi_test.txt
	.\$(SERIAL_BIN) inputs/p2048.txt outputs/test_2048.png >> outputs/tempi_test.txt
	
	@echo Esecuzione Test 4096x4096 in corso...
	@echo --- Test 4096x4096 --- >> outputs/tempi_test.txt
	.\$(SERIAL_BIN) inputs/p4096.txt outputs/test_4096.png >> outputs/tempi_test.txt
	
	@echo Esecuzione Test 8192x8192 in corso...
	@echo --- Test 8192x8192 --- >> outputs/tempi_test.txt
	.\$(SERIAL_BIN) inputs/p8192.txt outputs/test_8192.png >> outputs/tempi_test.txt
	
	@echo ">>> Tutti i test completati! I risultati sono in outputs/tempi_test.txt"

test_parallel: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_parallel.txt
	@echo TEST SCALABILITA' PARALLELA >> outputs/tempi_test_parallel.txt
	@echo ======================================= >> outputs/tempi_test_parallel.txt
	
	@echo Esecuzione Test Parallelo 512x512 in corso...
	@echo --- Test 512x512 --- >> outputs/tempi_test_parallel.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_par_512.png >> outputs/tempi_test_parallel.txt
	
	@echo Esecuzione Test Parallelo 1024x1024 in corso...
	@echo --- Test 1024x1024 --- >> outputs/tempi_test_parallel.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p1024.txt outputs/test_par_1024.png >> outputs/tempi_test_parallel.txt
	
	@echo Esecuzione Test Parallelo 2048x2048 in corso...
	@echo --- Test 2048x2048 --- >> outputs/tempi_test_parallel.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p2048.txt outputs/test_par_2048.png >> outputs/tempi_test_parallel.txt
	
	@echo Esecuzione Test Parallelo 4096x4096 in corso...
	@echo --- Test 4096x4096 --- >> outputs/tempi_test_parallel.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_par_4096.png >> outputs/tempi_test_parallel.txt
	
	@echo Esecuzione Test Parallelo 8192x8192 in corso...
	@echo --- Test 8192x8192 --- >> outputs/tempi_test_parallel.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_par_8192.png >> outputs/tempi_test_parallel.txt
	
	@echo ">>> Tutti i test paralleli completati! I risultati sono in outputs/tempi_test_parallel.txt"

# ============================================================
#  Test Speedup e Overhead (Confronto Diretto Seriale vs Parallelo)
# ============================================================

test_speedup_4096: all
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_speedup_4096.txt
	@echo TEST SPEEDUP E OVERHEAD (Risoluzione: 4096x4096) >> outputs/tempi_test_speedup_4096.txt
	@echo Valori thread testati: 1, 12, 512, 2048, 4096, 16384 >> outputs/tempi_test_speedup_4096.txt
	@echo ======================================= >> outputs/tempi_test_speedup_4096.txt
	
	@echo === TEMPI PARALLELI (DYNAMIC SCHEDULE) ===
	@echo Esecuzione Parallela con 1 Thread...
	@echo --- Parallelo: 1 Thread --- >> outputs/tempi_test_speedup_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_speedup_par_1.png 1 >> outputs/tempi_test_speedup_4096.txt
	
	@echo Esecuzione Parallela con 16 Thread...
	@echo --- Parallelo: 12 Thread --- >> outputs/tempi_test_speedup_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_speedup_par_12.png 12 >> outputs/tempi_test_speedup_4096.txt
	
	@echo Esecuzione Parallela con 512 Thread...
	@echo --- Parallelo: 512 Thread --- >> outputs/tempi_test_speedup_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_speedup_par_512.png 512 >> outputs/tempi_test_speedup_4096.txt
	
	@echo Esecuzione Parallela con 2048 Thread...
	@echo --- Parallelo: 2048 Thread --- >> outputs/tempi_test_speedup_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_speedup_par_2048.png 2048 >> outputs/tempi_test_speedup_4096.txt
	
	@echo Esecuzione Parallela con 4096 Thread...
	@echo --- Parallelo: 4096 Thread --- >> outputs/tempi_test_speedup_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_speedup_par_4096.png 4096 >> outputs/tempi_test_speedup_4096.txt
	
	@echo Esecuzione Parallela con 16384 Thread...
	@echo --- Parallelo: 16384 Thread --- >> outputs/tempi_test_speedup_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_speedup_par_16384.png 16384 >> outputs/tempi_test_speedup_4096.txt
	
	@echo ">>> Test di Speedup completati! I risultati sono in outputs/tempi_test_speedup_4096.txt"


test_speedup_512: all
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_speedup_512.txt
	@echo TEST SPEEDUP E OVERHEAD (Risoluzione: 512x512) >> outputs/tempi_test_speedup_512.txt
	@echo Valori thread testati: 1, 12, 512, 2048, 4096, 16384 >> outputs/tempi_test_speedup_512.txt
	@echo ======================================= >> outputs/tempi_test_speedup_512.txt
	
	@echo === TEMPI PARALLELI (DYNAMIC SCHEDULE) ===
	@echo Esecuzione Parallela con 1 Thread...
	@echo --- Parallelo: 1 Thread --- >> outputs/tempi_test_speedup_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_speedup_par_1.png 1 >> outputs/tempi_test_speedup_512.txt
	
	@echo Esecuzione Parallela con 16 Thread...
	@echo --- Parallelo: 12 Thread --- >> outputs/tempi_test_speedup_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_speedup_par_12.png 12 >> outputs/tempi_test_speedup_512.txt
	
	@echo Esecuzione Parallela con 512 Thread...
	@echo --- Parallelo: 512 Thread --- >> outputs/tempi_test_speedup_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_speedup_par_512.png 512 >> outputs/tempi_test_speedup_512.txt
	
	@echo Esecuzione Parallela con 2048 Thread...
	@echo --- Parallelo: 2048 Thread --- >> outputs/tempi_test_speedup_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_speedup_par_2048.png 2048 >> outputs/tempi_test_speedup_512.txt
	
	@echo Esecuzione Parallela con 4096 Thread...
	@echo --- Parallelo: 4096 Thread --- >> outputs/tempi_test_speedup_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_speedup_par_4096.png 4096 >> outputs/tempi_test_speedup_512.txt
	
	@echo Esecuzione Parallela con 16384 Thread...
	@echo --- Parallelo: 16384 Thread --- >> outputs/tempi_test_speedup_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_speedup_par_16384.png 16384 >> outputs/tempi_test_speedup_512.txt
	
	@echo ">>> Test di Speedup completati! I risultati sono in outputs/tempi_test_speedup_512.txt"
# ============================================================
#  Test Scheduling (Bilanciamento Carico a varie risoluzioni)
# ============================================================

test_schedule_512: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_schedule_512.txt
	@echo TEST SCHEDULING (Risoluzione: 512x512 - Threads: 12) >> outputs/tempi_test_schedule_512.txt
	@echo ======================================= >> outputs/tempi_test_schedule_512.txt
	
	@echo Esecuzione con scheduling STATIC (512x512)...
	@echo --- Scheduling: STATIC --- >> outputs/tempi_test_schedule_512.txt
	set OMP_SCHEDULE=static && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_sch_static_512.png 12 >> outputs/tempi_test_schedule_512.txt
	
	@echo Esecuzione con scheduling DYNAMIC (512x512)...
	@echo --- Scheduling: DYNAMIC --- >> outputs/tempi_test_schedule_512.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_sch_dynamic_512.png 12 >> outputs/tempi_test_schedule_512.txt
	
	@echo Esecuzione con scheduling GUIDED (512x512)...
	@echo --- Scheduling: GUIDED --- >> outputs/tempi_test_schedule_512.txt
	set OMP_SCHEDULE=guided && .\$(PARALLEL_BIN) inputs/p512.txt outputs/test_sch_guided_512.png 12 >> outputs/tempi_test_schedule_512.txt
	
	@echo ">>> Test di scheduling completati! Risultati in outputs/tempi_test_schedule_512.txt"

test_schedule_1024: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_schedule_1024.txt
	@echo TEST SCHEDULING (Risoluzione: 1024x1024 - Threads: 12) >> outputs/tempi_test_schedule_1024.txt
	@echo ======================================= >> outputs/tempi_test_schedule_1024.txt
	
	@echo Esecuzione con scheduling STATIC (1024x1024)...
	@echo --- Scheduling: STATIC --- >> outputs/tempi_test_schedule_1024.txt
	set OMP_SCHEDULE=static && .\$(PARALLEL_BIN) inputs/p1024.txt outputs/test_sch_static_1024.png 12 >> outputs/tempi_test_schedule_1024.txt
	
	@echo Esecuzione con scheduling DYNAMIC (1024x1024)...
	@echo --- Scheduling: DYNAMIC --- >> outputs/tempi_test_schedule_1024.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p1024.txt outputs/test_sch_dynamic_1024.png 12 >> outputs/tempi_test_schedule_1024.txt
	
	@echo Esecuzione con scheduling GUIDED (1024x1024)...
	@echo --- Scheduling: GUIDED --- >> outputs/tempi_test_schedule_1024.txt
	set OMP_SCHEDULE=guided && .\$(PARALLEL_BIN) inputs/p1024.txt outputs/test_sch_guided_1024.png 12 >> outputs/tempi_test_schedule_1024.txt
	
	@echo ">>> Test di scheduling completati! Risultati in outputs/tempi_test_schedule_1024.txt"

test_schedule_2048: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_schedule_2048.txt
	@echo TEST SCHEDULING (Risoluzione: 2048x2048 - Threads: 12) >> outputs/tempi_test_schedule_2048.txt
	@echo ======================================= >> outputs/tempi_test_schedule_2048.txt
	
	@echo Esecuzione con scheduling STATIC (2048x2048)...
	@echo --- Scheduling: STATIC --- >> outputs/tempi_test_schedule_2048.txt
	set OMP_SCHEDULE=static && .\$(PARALLEL_BIN) inputs/p2048.txt outputs/test_sch_static_2048.png 12 >> outputs/tempi_test_schedule_2048.txt
	
	@echo Esecuzione con scheduling DYNAMIC (2048x2048)...
	@echo --- Scheduling: DYNAMIC --- >> outputs/tempi_test_schedule_2048.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p2048.txt outputs/test_sch_dynamic_2048.png 12 >> outputs/tempi_test_schedule_2048.txt
	
	@echo Esecuzione con scheduling GUIDED (2048x2048)...
	@echo --- Scheduling: GUIDED --- >> outputs/tempi_test_schedule_2048.txt
	set OMP_SCHEDULE=guided && .\$(PARALLEL_BIN) inputs/p2048.txt outputs/test_sch_guided_2048.png 12 >> outputs/tempi_test_schedule_2048.txt
	
	@echo ">>> Test di scheduling completati! Risultati in outputs/tempi_test_schedule_2048.txt"

test_schedule_4096: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_schedule_4096.txt
	@echo TEST SCHEDULING (Risoluzione: 4096x4096 - Threads: 12) >> outputs/tempi_test_schedule_4096.txt
	@echo ======================================= >> outputs/tempi_test_schedule_4096.txt
	
	@echo Esecuzione con scheduling STATIC (4096x4096)...
	@echo --- Scheduling: STATIC --- >> outputs/tempi_test_schedule_4096.txt
	set OMP_SCHEDULE=static && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_sch_static_4096.png 12 >> outputs/tempi_test_schedule_4096.txt
	
	@echo Esecuzione con scheduling DYNAMIC (4096x4096)...
	@echo --- Scheduling: DYNAMIC --- >> outputs/tempi_test_schedule_4096.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_sch_dynamic_4096.png 12 >> outputs/tempi_test_schedule_4096.txt
	
	@echo Esecuzione con scheduling GUIDED (4096x4096)...
	@echo --- Scheduling: GUIDED --- >> outputs/tempi_test_schedule_4096.txt
	set OMP_SCHEDULE=guided && .\$(PARALLEL_BIN) inputs/p4096.txt outputs/test_sch_guided_4096.png 12 >> outputs/tempi_test_schedule_4096.txt
	
	@echo ">>> Test di scheduling completati! Risultati in outputs/tempi_test_schedule_4096.txt"

test_schedule_8192: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_schedule.txt
	@echo TEST SCHEDULING E BILANCIAMENTO CARICO >> outputs/tempi_test_schedule.txt
	@echo (Risoluzione: 8192x8192 - Threads: 12) >> outputs/tempi_test_schedule.txt
	@echo ======================================= >> outputs/tempi_test_schedule.txt
	
	@echo Esecuzione con scheduling STATIC (8192x8192)...
	@echo --- Scheduling: STATIC --- >> outputs/tempi_test_schedule.txt
	set OMP_SCHEDULE=static && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_sch_static.png 12 >> outputs/tempi_test_schedule.txt
	
	@echo Esecuzione con scheduling DYNAMIC (8192x8192)...
	@echo --- Scheduling: DYNAMIC --- >> outputs/tempi_test_schedule.txt
	set OMP_SCHEDULE=dynamic && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_sch_dynamic.png 12 >> outputs/tempi_test_schedule.txt
	
	@echo Esecuzione con scheduling GUIDED (8192x8192)...
	@echo --- Scheduling: GUIDED --- >> outputs/tempi_test_schedule.txt
	set OMP_SCHEDULE=guided && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_sch_guided.png 12 >> outputs/tempi_test_schedule.txt
	
	@echo ">>> Test di scheduling completati! I risultati sono in outputs/tempi_test_schedule.txt"

test_chunks: parallel
	-@mkdir outputs 2>nul
	@echo ======================================= > outputs/tempi_test_chunks.txt
	@echo TEST DIMENSIONE CHUNK (8192x8192, 12 Threads) >> outputs/tempi_test_chunks.txt
	@echo Valori testati: 1, 16, 64, 128, 256, 512 >> outputs/tempi_test_chunks.txt
	@echo ======================================= >> outputs/tempi_test_chunks.txt
	
	@echo === SCHEDULING STATIC ===
	@echo --- SCHEDULING: STATIC --- >> outputs/tempi_test_chunks.txt
	@echo Esecuzione STATIC con Chunk = 1...
	set OMP_SCHEDULE=static,1 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_stat_c1.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione STATIC con Chunk = 16...
	set OMP_SCHEDULE=static,16 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_stat_c16.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione STATIC con Chunk = 64...
	set OMP_SCHEDULE=static,64 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_stat_c64.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione STATIC con Chunk = 128...
	set OMP_SCHEDULE=static,128 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_stat_c128.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione STATIC con Chunk = 256...
	set OMP_SCHEDULE=static,256 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_stat_c256.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione STATIC con Chunk = 512...
	set OMP_SCHEDULE=static,512 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_stat_c512.png 12 >> outputs/tempi_test_chunks.txt
	
	@echo === SCHEDULING DYNAMIC ===
	@echo --- SCHEDULING: DYNAMIC --- >> outputs/tempi_test_chunks.txt
	@echo Esecuzione DYNAMIC con Chunk = 1...
	set OMP_SCHEDULE=dynamic,1 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_dyn_c1.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione DYNAMIC con Chunk = 16...
	set OMP_SCHEDULE=dynamic,16 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_dyn_c16.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione DYNAMIC con Chunk = 64...
	set OMP_SCHEDULE=dynamic,64 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_dyn_c64.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione DYNAMIC con Chunk = 128...
	set OMP_SCHEDULE=dynamic,128 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_dyn_c128.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione DYNAMIC con Chunk = 256...
	set OMP_SCHEDULE=dynamic,256 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_dyn_c256.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione DYNAMIC con Chunk = 512...
	set OMP_SCHEDULE=dynamic,512 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_dyn_c512.png 12 >> outputs/tempi_test_chunks.txt
	
	@echo === SCHEDULING GUIDED ===
	@echo --- SCHEDULING: GUIDED --- >> outputs/tempi_test_chunks.txt
	@echo Esecuzione GUIDED con Chunk minimo = 1...
	set OMP_SCHEDULE=guided,1 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_gui_c1.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione GUIDED con Chunk minimo = 16...
	set OMP_SCHEDULE=guided,16 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_gui_c16.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione GUIDED con Chunk minimo = 64...
	set OMP_SCHEDULE=guided,64 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_gui_c64.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione GUIDED con Chunk minimo = 128...
	set OMP_SCHEDULE=guided,128 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_gui_c128.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione GUIDED con Chunk minimo = 256...
	set OMP_SCHEDULE=guided,256 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_gui_c256.png 12 >> outputs/tempi_test_chunks.txt
	@echo Esecuzione GUIDED con Chunk minimo = 512...
	set OMP_SCHEDULE=guided,512 && .\$(PARALLEL_BIN) inputs/p8192.txt outputs/test_gui_c512.png 12 >> outputs/tempi_test_chunks.txt
	
	@echo ">>> Test completati! I risultati sono in outputs/tempi_test_chunks.txt"
# ============================================================
#  Pulizia
# ============================================================

clean:
	-@del /Q /F $(SERIAL_BIN) $(PARALLEL_BIN) 2>nul
	@echo ">>> File binari rimossi."

help:
	@echo Targets disponibili:
	@echo   mingw32-make                  - compila tutto (seriale e parallelo)
	@echo   mingw32-make run              - esegue il codice seriale
	@echo   mingw32-make run_parallel     - esegue il codice parallelo
	@echo   mingw32-make test             - esegue i test di scalabilita (512, 1024, 2048, 4096, 8192) in serie
	@echo   mingw32-make clean            - rimuove i file eseguibili
	@echo   mingw32-make test_parallel    - esegue i test di scalabilita (512, 1024, 2048, 4096, 8192) in parallelo
	@echo   mingw32-make test_speedup_4096  - esegue i test di scalabilita variando i thread a 4096x4096
	@echo   mingw32-make test_speedup_512  - esegue i test di scalabilita variando i thread a 512x512
	@echo   mingw32-make test_schedule_8192 - esegue i test di scheduling a 8192x8192
	@echo   mingw32-make test_schedule_512  - esegue i test di scheduling a 512x512
	@echo   mingw32-make test_schedule_1024 - esegue i test di scheduling a 1024x1024
	@echo   mingw32-make test_schedule_2048 - esegue i test di scheduling a 2048x2048
	@echo   mingw32-make test_schedule_4096 - esegue i test di scheduling a 4096x4096
	@echo   mingw32-make test_chunks      - esegue i test al variare del numero di chunk per scheduling dinamico, guidato e statico