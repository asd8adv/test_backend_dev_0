### 5 самых активных устройств
``` SQL
SELECT device, COUNT(*) AS login_count
FROM players
GROUP BY device
ORDER BY login_count DESC
LIMIT 5;
```
### Среднее число логинов в день за последние 7 дней
``` SQL
SELECT AVG(daily_count) AS avg_logins_per_day
FROM (
    SELECT DATE(login_time) AS login_date, COUNT(*) AS daily_count
    FROM players
    WHERE login_time >= CURDATE() - INTERVAL 7 DAY
    GROUP BY login_date
) AS daily_stats;
```