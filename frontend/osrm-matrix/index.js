const fs = require('fs/promises');
const path = require('path');

// Путь к файлу с исходными точками
const INPUT_FILE = './request.json';
// Путь к итоговому файлу с матрицей
const OUTPUT_FILE = './init_task_distribution.json';

/**
 * Читает JSON-файл с точками и приводит их к единому виду.
 */
async function loadPoints(filePath) {
    const raw = await fs.readFile(filePath, 'utf-8');
    const data = JSON.parse(raw);

    // Проверяем, что есть массив tasks
    if (!data.tasks || !Array.isArray(data.tasks) || data.tasks.length === 0) {
        throw new Error('Файл не содержит непустого массива "tasks".');
    }

    const coordinates = [];
    const pointsInfo = [];

    for (let i = 0; i < data.tasks.length; i++) {
        const task = data.tasks[i];

        // Извлекаем координаты из вложенного объекта position
        if (!task.position || typeof task.position.latitude !== 'number' || typeof task.position.longitude !== 'number') {
            throw new Error(`Задача №${i} (UID: ${task.task_uid || 'неизвестен'}) не содержит корректных координат в "position".`);
        }

        const lat = task.position.latitude;
        const lon = task.position.longitude;

        // OSRM требует строгий порядок: [долгота, широта]
        coordinates.push([lon, lat]);

        // Сохраняем информацию о точке для итогового файла
        pointsInfo.push({
            index: i,
            task_uid: task.task_uid || `Точка ${i}`,
            task_region: task.task_region || 'неизвестно',
            lat: lat,
            lon: lon,
            time_window: task.time_window || null
        });
    }

    return { coordinates, pointsInfo };
}

/**
 * Получает матрицу времени в пути между всеми точками.
 */
async function getTravelTimeMatrix(coordinates) {
    if (coordinates.length > 100) {
        throw new Error('Публичный OSRM поддерживает не более 100 точек.');
    }

    const coordsString = coordinates.map(([lon, lat]) => `${lon},${lat}`).join(';');
    const url = `https://router.project-osrm.org/table/v1/transit/${coordsString}?annotations=duration`;

    const response = await fetch(url);

    if (!response.ok) {
        throw new Error(`Ошибка HTTP при запросе к OSRM: ${response.status}`);
    }

    const data = await response.json();

    if (data.code !== 'Ok') {
        throw new Error(`Ошибка API OSRM: ${data.code} — ${data.message || 'Нет описания'}`);
    }

    return data.durations;
}

/**
 * Сохраняет матрицу и метаданные в JSON-файл.
 */
async function saveMatrixToJson(filePath, coordinates, pointsInfo, matrix) {
    const output = {
        generatedAt: new Date().toISOString(),
        source: 'OSRM (router.project-osrm.org)',
        units: 'seconds',
        pointCount: coordinates.length,
        points: pointsInfo,
        matrix: matrix,
    };

    await fs.writeFile(filePath, JSON.stringify(output, null, 2), 'utf-8');
    console.log(`Матрица успешно сохранена в: ${path.resolve(filePath)}`);
}

(async () => {
    try {
        console.log(`Читаю точки из файла: ${path.resolve(INPUT_FILE)}`);
        const { coordinates, pointsInfo } = await loadPoints(INPUT_FILE);
        console.log(`Загружено точек (задач): ${coordinates.length}`);

        console.log('Запрашиваю матрицу у OSRM...');
        const matrix = await getTravelTimeMatrix(coordinates);

        await saveMatrixToJson(OUTPUT_FILE, coordinates, pointsInfo, matrix);
    } catch (err) {
        console.error('Ошибка:', err.message);
    }
})();
